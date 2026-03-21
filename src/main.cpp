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
