#pragma once

#include <iosfwd> 
#include <string> 
#include <unordered_map> 

namespace ledgerware::cli {

	enum class CommandType {
		Help, 
		Migrate, 
		Seed, 
		Receive, 
		Stock, 
		Unknown, 
		None
	};

	struct Command {
		CommandType type{ CommandType::None }; 
		std::string raw_command; // example "receive" 
		std::unordered_map<std::string, std::string> flags; // sku-> A1, loc-> WH, qty->20, user->martin
	 };
		
	struct ParseResult {
		bool ok{ false }; 
		int exit_code{ 1 }; 
		std::string error; 
		Command command{}; 
	};

	// Parse argv into a command + flags. Does NOT execute anything. 
	ParseResult parse(int argc, char** argv); 

	// Print help/usage text
	void print_help(std::ostream& os); 


} // namespace ledgerware::cli