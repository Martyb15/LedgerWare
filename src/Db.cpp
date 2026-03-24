#include "Db.hpp"

namespace ledgerware::db {

	// ------- Constructor ---------------
	Database::Database(std::string_view path) {
		std::string path_str(path); 

		int rc = sqlite3_open(path_str.c_str(), &db_);  // return code -> rc
		
		if (rc != SQLITE_OK) {
			std::string msg = db_ ? sqlite3_errmsg(db_) : "unknown error"; 
			sqlite3_close(db_);                         // safe even if db_ is null
			db_ = nullptr; 
			throw std::runtime_error("Failed to open database: " + msg); 
		}
		exec("PRAGMA foreign_keys = ON"); 
	}

	// ------- Destructor -----------------
	Database::~Database() {
		if (db_) {
			sqlite3_close(db_);
		}
	}

	
	// ----- Move Constructor ------
	//  Transfer the connection from 'other' into 'this'. 
	//  After the move, 'other' is empty - it won't close anything.
    //
	// Database a("ledger.db");               // a owns the connection
	// Database b = std::move(a);			  // b now owns it
	// a.db_ == nullptr (safe, destructor will skip close)
	// b.db_ == the live connection
	Database::Database(Database && other) noexcept
		: db_{ other.db_ }
	{
		other.db_ = nullptr; 
	}


	
}