#ifndef TOKENTYPEPRINT_H
#define TOKENTYPEPRINT_H

#include <string>
#include "Token.h"

using namespace std;

string		 Toktpye_forPrint[Token::NumTokens] = {
													"EndOfInputToken",
													"UnknownToken",
													"VariableToken",
													"ConstantToken",
													"KeywordEvalToken",
													"KeywordExpToken",
													"KeywordValueToken",
													"OperatorPlusToken",
													"OperatorMinusToken",
													"OperatorAsteriskToken",
													"OperatorSlashToken",
													"SymbolOpenParenToken",
													"SymbolCloseParenToken",
													"SymbolSemiColonToken" 
	                                               };

#endif