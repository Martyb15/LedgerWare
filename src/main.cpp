#include <fmt/core.h> 
#include <sqlite3.h>

#include <filesystem> 
#include <fstream> 
#include <stdexcept> 
#include <string> 
#include <vector> 
#include <iostream>

static void exec_sql(sqlite3* db, const std::string& sql) {
	char* err = nullptr; 
	//rc "return code" -> 0 = ok/success
	int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err); 
	if (rc != SQLITE_OK) {
		std::string msg = err ? err : "Unknown SQLite error";
		sqlite3_free(err); 
		throw std::runtime_error("SQLite exec failed: " + msg);
	}
} 

static std::string read_file(const std::filesystem::path& p) {
	std::ifstream file_in(p, std::ios::binary); 
	if (!file_in) throw std::runtime_error("Failed to open file: " + p.string());
	return std::string((std::istreambuf_iterator<char>(file_in)), std::istreambuf_iterator<char>()); 
}

static void open_db(sqlite3** out_db, const std::string& path) {
	sqlite3* db = nullptr; 
	int rc = sqlite3_open(path.c_str(), &db); 
	if (rc != SQLITE_OK) {
		std::string msg = db ? sqlite3_errmsg(db) : "Unknown error (db is null)";
		if (db) sqlite3_close(db); 
		throw std::runtime_error("Failed to open DB: " + msg); 
	}
}