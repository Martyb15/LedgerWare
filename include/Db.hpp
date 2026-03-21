#pragma once 

#include <sqlite3.h>

#include <stdexcept>
#include <string>
#include <string_view> 

namespace ledgerware::db {
	class Database {

	public: 
		explicit Database(std::_String_val path); 
		~Database(); 

		Database(const Database&) = delete; 
		Database& operator = (const Database&) = delete; 

		Database(Database&& other) noexcept; 
		Database& operator=(Database&& other) noexcept; 
		void exec(std::_String_val sql); 
		sqlite3* raw() noexcept; 
	
	
	private:
		sqlite3* db_{ nullptr }; 

	};

} 