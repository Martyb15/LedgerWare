#include "commands.hpp"
#include <iostream> 

namespace ledgerware::commands {


	int migrate() {
		std::cout << "[migrate] handler reached (LW-2.2 will implement)\n";
		return 0;
	}

	int seed() {
		std::cout << "[seed] handler reached (LW-2.3 will implement)\n"; 
		return 0;
	}


	int receive(const ledgerware::cli::Command& cmd) {
		std::cout << "[receive] handler reached\n"; 
		auto get = [&](const char* k)  -> std::string {
			auto it = cmd.flags.find(k);
			return (it == cmd.flags.end()) ? "" : it->second;
		}; 
		std::cout << "	sku=" << get("sku")
			<< " loc=" << get("loc")
			<< " qty=" << get("qty")
			<< " user=" << get("user") << "\n"; 
		return 0;
	}

	int stock(const ledgerware::cli::Command& cmd) {
		std::cout << "[stock] handler reached\n"; 
		auto get = [&](const char* k) -> std::string {
			auto it = cmd.flags.find(k);
			return (it == cmd.flags.end()) ? "" : it->second;
		}; 
		std::cout << "  sku=" << get("sku")
			<< " loc=" << get("loc") << "\n"; 
		return 0; 
	}
} // namspace ledgerware::commands