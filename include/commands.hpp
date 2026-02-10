#pragma once
#include "cli.hpp"



namespace ledgerware::commands {
	int migrate(); 
	int seed(); 
	int receive(const ledgerware::cli::Command& cmd); 
	int stock(const ledgerware::cli::Command& cmd); 


}// namespace ladgerware::commands