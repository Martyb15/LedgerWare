#pragma once 

#include <sqlite3.h>

#include <stdexcept>
#include <string>
#include <string_view> 

namespace ledgerware::db {

// Resource Acquisition Is Initialization
// The idea: 
//		- The constructor acquires the resource (opens the DB) 
//	    - The Destructor releases the resource (closes the DB) 
//      - You never manually call sqlite3_close, it happens automaticallly when the database object goes out of scope. 
//
// Why it matters: 
//      - If an exception is thrown, the destructor still runs -> no leak
//      - If you return early from a function, destructor still runs -> no leak
//      - You literally cannot forget to close the DB
//
// This class is "move-only": 
//	    - You CAN transfer ownership: Database b = std:: move(a); 
// 	    - Now b owns the connection and a is empty. 
// 	    - You CANNOT copy: Database b = a; <- compiler error because two objects closing the same pointer = crash
// 
//
//
//
	class Database {

	public: 
		// ----- Constructor: acquire the resource -----
		// Opens a SQLite database at the given path. 
		// If it fails, throws an exception, so if you have a Database object,
		// you KNOW it holds a valid connection. 
		explicit Database(std::string_view path); 

		// ---- Destructor: release the resource -------
		// Closes the SQLite connection automatically. 
		// Called when the object goes out of scope, or is deleted, or the program
		// exits - you never call this yourself. 
		~Database(); 


		// ---- Delete -----
		// Two Database objects pointing at the same sqlite* would both try to 
		// close it -> double-free crash. 
		// Deleting these makes the compiler reject any attempt to copy. 
		// 
		// Database a('ledger.db'); 
		// Database b = a;         <- COMPILE ERROR (this is what we want)
		Database(const Database&) = delete; 
		Database& operator = (const Database&) = delete; 

		// --- Move operations: transfer ownership ---- 
		// "Moving" means one object hands its guts to another. 
		// After the move, the source object is empty (db_ == nullptr)
		// and the destination owns the connection.
		// 
		// noexcept tells the compiler these won't throw, which enables 
		// optimization (e.g. std::vector can move instead of copy). 
		Database(Database&& other) noexcept; 
		Database& operator=(Database&& other) noexcept; 

		// ----- exec: run SQL  that doesn't return rows ------ 
		// Use for CREATE TABLE, INSERT, UPDATE, DELET, PRAGMA, etc. 
		// Throws on error with the SQLite error message. 
		void exec(std::string_view sql);

		// ----- raw: access the underlying pointer ------- 
		// Sometimes you need the raw sqlite3* for prepared statements
		// or other SQLite API calls. This gives you access without giving
		// up ownership - the Database still manages the lifetime. 
		sqlite3* raw() noexcept; 
	
	
	private:
		sqlite3* db_{ nullptr }; 
		// Why initialize to nullptr? 
		// If the constructor throws, the destructor won't run on a 
		// partially constructed object. But for moved-from objects, 
		// we set db_ = nullptr so the destructor knows there's nothing to close. 


	};

} 