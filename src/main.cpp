//#include <fmt/core.h> 
//#include <sqlite3.h>

//#include <filesystem> 
//#include <fstream> 
//#include <stdexcept> 
//#include <string> 
//#include <vector> 
#include "cli.hpp"
#include "commands.hpp"

#include <iostream>


int main(int argc, char** argv) {
	auto parsed = ledgerware::cli::parse(argc, argv); 

	// Parse error (unknown command, missing flag values, etc.)
	if (!parsed.ok) {
		if (!parsed.error.empty()) {
			std::cerr << parsed.error << "\n\n";
		}
		ledgerware::cli::print_help(std::cerr); 
		return parsed.exit_code;  //usually 2
	}

	// Help should never go through the command switch.
	if (parsed.command.type == ledgerware::cli::CommandType::Help) {
		ledgerware::cli::print_help(std::cout); 
		return parsed.exit_code; // 0 for help, 1 for no-args 
	}

	using ledgerware::cli::CommandType; 

	switch (parsed.command.type) {
		case CommandType::Migrate:  return ledgerware::commands::migrate(); 
		case CommandType::Seed:		return ledgerware::commands::seed(); 
		case CommandType::Receive:  return ledgerware::commands::receive(parsed.command); 
		case CommandType::Stock:    return ledgerware::commands::stock(parsed.command);

		default: 
			std::cerr << "Unknown command.\n\n"; 
			ledgerware::cli::print_help(std::cerr);
			return 2; 
	}
}




/*
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
*/