#include "cli.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace ledgerware::cli {

	static bool starts_with(std::string_view s, std::string_view prefix) {
		// function checks whether a token begins with a certain prefix. Example -- 
		return s.size() >= prefix.size() && s.substr(0, prefix.size()) == prefix;
	}

	static bool is_help_token(std::string_view s) {
		// is the user input attempting to ask for help
		return (s == "help" || s == "--help" || s == "-h");
		// possibly need to handle uppercase. 
	}

	static ParseResult make_error(std::string msg, int code = 2) {
		ParseResult r;
		r.ok = false;
		r.exit_code = code;
		r.error = std::move(msg);
		return r;
	}

	ParseResult parse(int argc, char** argv) {
		ParseResult out;

		if (argc <= 1) {
			out.ok = true;
			out.exit_code = 1;    // no args -> help + non-zero
			out.command.type = CommandType::Help;
			return out;
		}

		std::vector<std::string_view> args;
		args.reserve(static_cast<size_t>(argc)); // Reserve since we know the amount of elements to expect when running
		// We will iterate and extract all argv elements and place them into args vector -- skipping argv[0]
		for (int i = 1; i < argc; i++) args.emplace_back(argv[i]); // first value args[0] is program name or path 

		// help anywhere
		for (auto a : args) {
			if (a == "help" || a == "--help" || a == "-h") {
				out.ok = true;
				out.exit_code = 0;
				out.command.type = CommandType::Help;
				return out;
			}
		}

		// first token is command
		std::string_view cmd = args[0];
		out.command.raw_command = std::string(cmd);

		if (is_help_token(cmd)) out.command.type = CommandType::Help;
		else if (cmd == "migrate") out.command.type = CommandType::Migrate;
		else if (cmd == "seed") out.command.type = CommandType::Seed;
		else if (cmd == "receive") out.command.type = CommandType::Receive;
		else if (cmd == "stock") out.command.type = CommandType::Stock;
		else {
			return make_error("Unknown command: " + std::string(cmd), 2);
		}

		// parse flags: supports --k v and --k=v
		for (size_t i = 1; i < args.size(); ) {
			std::string_view token = args[i];

			if (!starts_with(token, "--")) {
				return make_error("Unexpected argument: " + std::string(token), 2);
			}
			// ? std::string_view allows us to efficiently extract the key and value without creating new string copies (allocations). 
			// ? npos is th largest possible number that a computer can store in a size_t variable 18,446,744,709,551,615 or sometimes it is -1. 

			auto eq = token.find("=");
			if (eq != std::string_view::npos) { // npos (no position) if find() returns 0 it found it at the start if it doesn't find the character at all, it can't return 0 (because 0 is a valid indext) instead, it return npos to say "I searched everywhere and found nothing"
				std::string_view key = token.substr(2, eq - 2);
				std::string_view val = token.substr(eq + 1);
				if (val.empty()) return make_error("Missing value for flag: --" + std::string(key), 2);
				out.command.flags[std::string(key)] = std::string(val);
				++i;
				continue;
			}

			std::string_view key = token.substr(2);
			if (i + 1 >= args.size()) return make_error("Missing value for flag: --" + std::string(key), 2);

			std::string_view val = args[i + 1];
			if (starts_with(val, "--")) return make_error("Missing value for flag: --" + std::string(key), 2);

			out.command.flags[std::string(key)] = std::string(val);
			i += 2;
		}

		out.ok = true;
		out.exit_code = (out.command.type == CommandType::Help) ? 0 : 0;
		return out;
	}

void print_help(std::ostream& os) {
	os <<
R"(LedgerWare - inventory ledger + snapshot demo

Usage:
  ledgerware <command> [options]

Commands:
  migrate                 Create/update database schema (safe to rerun)
  seed                    Insert sample product/location (no duplicates)
  receive  --sku <SKU> --loc <LOC> --qty <N> --user <USER>
                          Record a receive event and update stock snapshot
  stock    --sku <SKU> --loc <LOC>
                          Show stock snapshot totals
  help                    Show this help

Examples:
  ledgerware migrate
  ledgerware seed
  ledgerware receive --sku A1 --loc WH --qty 20 --user martin
  ledgerware stock --sku A1 --loc WH
)";
	}

}// namespace ledgerware::cli