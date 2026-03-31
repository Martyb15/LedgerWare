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


	// ----- Move Assignment ------
	// Same idea, but 'this' might already own a connection. 
	// We need to close our existing one before taking the new one. 
	// Database a("one.db"); 
	// Database b("two.db'); 
	// a = std::move(b);      // close one.db, a now owns two.db

	Database& Database::operator=(Database&& other) noexcept {
		if (this != &other) {
			if (db_) {
				sqlite3_close(db_);
			}
			db_ = other.db_;
			other.db_ = nullptr; 
		}
		return *this; 
	}



	// --- exec ---------
	// Runs a SQL statement (or multimple separated by semicolons). 
	// Throws on any error so you don't accidentally ignore failures. 

	void Database::exec(std::string_view sql) {
		char* err = nullptr; 
		std::string sql_str(sql); 

		int rc = sqlite3_exec(db_, sql_str.c_str(), nullptr, nullptr, &err); 

		if (rc != SQLITE_OK) {
			std::string msg = err ? err : "unknown SQLite error"; 
			sqlite3_free(err);                        // SQLite allocated this, we must free it
			throw std::runtime_error("SQL error: " + msg); 
		}
	}

	// ------ raw ----------
	// Returns the raw ponter for advanced SQLite API calls. 
	// The Database objects still owns it - don't close or free it. 
	
	sqlite3* Database::raw() noexcept {
		return db_; 
	}

	
} 