#pragma once

#include <iosfwd> 
#include <string> 
#include <unordered_map> 

namespace ledgerware::cli {
// ---------------------------------------------------
// Using nested namespaces (ledgerware::cli) keeps the CLI code separeate 
// from other parts of the app (like ledgerware::db or ledgerware::commands). 
// ---------------------------------------------------


	// ---- CommandType -------------
	// An "enum class" (scoped enum) that represents which command the user typed.  
	// Unlike a plain enum, enum class: 
	//    - Won't implicitly convert to int (safer) 
	//    - Won't leak names into the surrounding scope (you must write 
	//         CommantType::Migrate, not just Migrate)
	// 
	// None     = no command parsed yet (default state) 
	// Unknown  = user typed something we don't recognize
	// Help     = user asked for help (or gave no args) 
	// 
	// This is used in main.cpp's switch statement to dispatch to the righ handler function. 
	enum class CommandType {
		Help, 
		Migrate, 
		Seed, 
		Receive, 
		Stock, 
		Unknown, 
		None
	};



	// ------------------------------------- Command --------------------------------------------------
	// Holds everything we know about what the user asked for.                                        |
	// This is a pure data struct - it doesn't DO anything, it just                                   |
	// carries information from the parser to the dispatcher.                                         |
	//                                                                                                |
	// Example: "ledgerware receive --sku A1 --loc WH --qty 20 --user martin"                         |
	//		type         = CommandType::Recieve                                                       |
	//      raw_command  = "receive"                                                                  |
	//      flags        = {"sku":"A1", "loc":"WH", "qty":"20", "user":"martin" }                     |
	//                                                                                                |
	// Why raw_command AND type?                                                                      |
	//		type is what the code switches on (fast, type-safe).                                      |
	//		raw_command preserves teh exact string for error messages.                                |
	// -----------------------------------------------------------------------------------------------|
	struct Command {                                                                           //     |
		CommandType type{ CommandType::None };                                                 //     |
		// Brace initialization { CommandType::None } sets a default.                                 |
		// If you create a Command without specifying type, it starts as None - not                   |
		// some random garbage value                                                                  |

		std::string raw_command; // example "receive"                                                 |
		// The literal command string the user typed, e.g. "receive".                                 |
		// Usefule for error messages like: "Unknown command: frobnicate"                             |



		std::unordered_map<std::string, std::string> flags;                                     //    |
		// sku-> A1, loc-> WH, qty->20, user->martin                                                  |
		// Key-value pairs from the command line flags.                                               |
		//                                                                                            |
		// Why unordered_map instead of map?                                                          |
		//	  unordered_map uses a hash table -> O(1) average lookup.                                 |
		//    map uses a red-black tree -> O(log n) lookup.                                           |
		//    For a handful of CLI flags either is fine, but unordered_map is the idiomatic           |
		//    choice when you don't need sorted keys.                                                 |
		//                                                                                            |
		// Why <string, string> instead of <string_view, string_view>?                                |
		//	  Because string_view doesn't own its data - it just points at memory                     |
		//    owned by someone else (argv in this case).                                              |
		//    Storing string_views would be fragile if argv's lifetime ever changed. Stringown their  |
		//    data, so they're safe to pass around and store.                                         |
	 };                                                                                          //   |
 // --------------------------------------------------------------------------------------------------|



// -------------------------------------- ParseResult ------------------------------------------------|
// The return value of parse(). Bundles together:													  |
// 	   - Did parsing succeed? (ok)																	  |
// 	   - What exit code should the program use? (exit_code)											  |
// 	   - If it failed, why? (error)																	  |
// 	   - The parsed command (command)																  |
//																									  |
// This pattern (returning a result struct instead of throwing or using out-parameters) is common     |
// in C++. It keeps the caller in control: they decide how to handle errors.                          |
//																									  |
// Example usage in main.cpp:																	      | 
// 	   auto parsed = ledgerware::cli::parse(argc, argv);											  |
//     if (!parsed.ok) {																			  |
//		  std::cerr << parsed.error << "\n";                                                          |
// 	      return parsed.exit_code;                                                                    |
// 	   }                                                                                              |
// ---------------------------------------------------------------------------------------------------|
	struct ParseResult {                                                                          //  |
		bool ok{ false };                                                                         //  |
		int exit_code{ 1 };                                                                       //  |
		std::string error;                                                                        //  |
		Command command{};                                                                        //  |
	};                                                                                            //  |
// ok: Defaults to false. Th eparser sets this to truen only when everything is valid.                |
// exit_code: 0 = success  1 = general error (no args)   2 = usage error (bad flags, unknown command  |
// error: Human-readable error message. Empy on success. Example "missing value for flag: --sku"      |
// command{}: The parsed command data. Only meaningful when ok == true.                               |
//			  The {} gives it default values (type=None, empty flags).                                |
// ---------------------------------------------------------------------------------------------------|	 





	// Parse argv into a command + flags. Does NOT execute anything.                                  |
	ParseResult parse(int argc, char** argv);                                                     //  |
	                                                                                              //  |
	// Print help/usage text                                                                      //  |
	void print_help(std::ostream& os);                                                            //  |

} // namespace ledgerware::cli 