#include "Statement.h"
#include <iostream>
         
Token		          Statement::Toke;               
MathTree	          Statement::MT;                 
Variable::ValueType   Statement::MathTreeResult; 
int                   Statement::Which;              
bool                  Statement::SignVarVal      (false);  
bool                  Statement::PlusSign;           
int                   Statement::NumParen        (0);  

Statement::StateFunc  Statement::StateFunctions[] = {
                                                    NotYetImplimented,             // 0
                                                    GotEndOFFile,                  // 1
                                                    GotInvalidStatement,           // 2
                                                    GotEvalToken,                  // 3
                                                    GotExpToken,                   // 4
                                                    GotValueToken,                 // 5
                                                    GotOperandToken,               // 6
                                                    GotSignToken,                  // 7
                                                    GotOpenPareToken,              // 8
                                                    GotEndofStatement,             // 9
                                                    GoToSkipToSemicolon,           // 10
                                                    ContinueSkipToSemicolon,       // 11
                                                    GotOperatorToken,              // 12
                                                    GotCloseParenToken,            // 13
                                                    GotExpStatement,               // 14
													GotUnexpectedSemicolon,        // 15
                                                    GotVarName,                    // 16
                                                    GotVarSign,                    // 17
                                                    GotVarValue,                   // 18
                                                    GotVarValStetement,            // 19 
                                                    GotEvalStatement               // 20       
                                                    };

int			Statement::StateTable[Token::NumTokens][Statement::NumStates] = {   
	                                                    //                             StartStatement        AfterExpToken          SkipToSemicolon         AfterOperandToken       AfterOperatorToken       AfterOpenPareToken        AfterValueToken          AfterVarName        GotVarSign       AfterVarValue      AfterEvalToken 
                                                        /*EndOfInputToken       */  {        1,                    1,                      1,                      1,                      1,                        1,                       1,                     1,                 1,               1,                  1,},    
                                                        /*UnknownToken          */  {        2,                    10,                     11,                     10,                     10,                       10,					  11,                    11,                11,              11,                 11},   
                                                        /*VariableToken         */  {        2,                    6,                      11,                     10,                     6,                        6,					      16,                    11,                11,              11,                 11},    
                                                        /*ConstantToken         */  {        2,                    6,                      11,                     10,                     6,                        6,						  11,                    18,                18,              11,                 11},    
                                                        /*KeywordEvalToken      */  {        3,                    10,                     11,                     10,                     10,                       10,					  11,                    11,                11,              11,                 11},   
                                                        /*KeywordExpToken       */  {        4,                    10,                     11,                     10,                     10,						 10,					  11,                    11,                11,              11,                 11},                      
                                                        /*KeywordValueToken     */  {        5,                    10,                     11,                     10,                     10,						 10,					  11,                    11,                11,              11,                 11},  
                                                        /*OperatorPlusToken     */  {        2,                    7,                      11,                     12,                     7,						 7,						  11,                    17,                11,              11,                 11},   
                                                        /*OperatorMinusToken    */  {        2,                    7,                      11,                     12,                     7,						 7,						  11,                    17,                11,              11,                 11},   
                                                        /*OperatorAsteriskToken */  {        2,                    10,                     11,                     12,                     10,						 10,					  11,                    11,                11,              11,                 11},   
                                                        /*OperatorSlashToken    */  {        2,                    10,                     11,                     12,                     10,						 10,					  11,                    11,                11,              11,                 11},   
                                                        /*SymbolOpenParenToken  */  {        2,                    8,                      11,                     10,                     8,						 8,						  11,                    11,                11,              11,                 11},   
                                                        /*SymbolCloseParenToken */  {        2,                    10,                     11,                     13,                     10,						 13,					  11,                    11,                11,              11,                 11},   
                                                        /*SymbolSemiColonToken  */  {        2,                    9,                      9,                      14,                     15,						 15,					  9,                     2,                 2,               19,                 20}      
                                                                            };

void Statement::Build (const char * fname)
	{
    StatementStates  CurrentState (StartStatement);
    Toke.OpenFile (WCS_String (fname));
	do	{
		CurrentState = StateFunctions [StateTable [Toke.Build().GetType ()] [CurrentState]] (*this);
        } while (CurrentState != StatementComplete);
    Toke.CloseFile ();
	}

Statement::StatementStates Statement::GotEvalToken (Statement & S)
    {
    return AfterEvalToken;
    }

Statement::StatementStates Statement::GotExpToken (Statement &)
    {
    ClearEverything ();
    return AfterExpToken;
    }

Statement::StatementStates Statement::GotValueToken (Statement &)
    {
    return AfterValueToken;
    }

Statement::StatementStates Statement::GotOperandToken (Statement &)
    {
    MT.InsertOperand (Toke);
    return AfterOperandToken;
    }

Statement::StatementStates Statement::GotSignToken (Statement &)
    {
    Toke.SetPrecedence (Operator::UnaryPlusMinusPrecedence);
    Toke.SetNumIncludingParen (NumParen);
    MT.InsertUniaryOperator (Toke);
	return AfterOperatorToken;
    }

Statement::StatementStates Statement::GotOpenPareToken (Statement &)
    {
    NumParen ++;
    return AfterOpenPareToken;
    }

Statement::StatementStates Statement::GotEndofStatement (Statement &) 
    {
    return StartStatement;
    }

Statement::StatementStates Statement::GoToSkipToSemicolon (Statement &)
    {
    ClearEverything ();
    return SkipToSemicolon;
    }

Statement::StatementStates Statement::ContinueSkipToSemicolon (Statement &)
    {
    return SkipToSemicolon;
    }

Statement::StatementStates Statement::GotOperatorToken (Statement &)
    {
    Toke.SetNumIncludingParen (NumParen);
    MT.InsertBinaryOperator (Toke);
    return AfterOperatorToken;
    }

Statement::StatementStates Statement::GotCloseParenToken (Statement &)
    {
    if (--NumParen < 0)
		{
		ClearEverything ();
		std::cout << "Invalid expression, please build a new math tree." << endl;
		NumParen = 0;  
		return SkipToSemicolon;
		}
	else
		return AfterOperandToken;
    }

Statement::StatementStates Statement::GotExpStatement (Statement &)
    {
	if (NumParen != 0)
		{
		ClearEverything ();
		std::cout << "Invalid expression, please build a new math tree." << endl;
		NumParen = 0;  
		}
    else
		std::cout << "****** Now reading a new expression... ******" << endl;
    return StartStatement;
    }

Statement::StatementStates Statement::GotUnexpectedSemicolon (Statement &)
    {
    ClearEverything ();
    return StartStatement;
    }

Statement::StatementStates Statement::GotVarName (Statement &)
	{
	if ((Toke.GetWhich () < 0) || (Toke.GetWhich () > 99))
		{
		std::cout << "Variable is out of boundary." << endl;
		return SkipToSemicolon;
		}
    else
        {
        Which = Toke.GetWhich();
        return AfterVarName;
        }
	}

Statement::StatementStates Statement::GotVarSign (Statement &)
    {
	SignVarVal = true;
    if (Toke.GetType () == Token::OperatorPlusToken)
        PlusSign = true;
    else
        PlusSign = false;
    return AfterVarSign;
    }

Statement::StatementStates Statement::GotVarValue (Statement &)
    {
    return AfterVarValue;
    }

Statement::StatementStates Statement::GotVarValStetement (Statement &)
    {
    Token    TemToken (Token::VariableToken, Which);
	if (SignVarVal)
		{
		if (PlusSign)
			TemToken.SetValue (Toke.GetValue ());
		else
			TemToken.SetValue(0 - Toke.GetValue ());
		}
		else
			TemToken.SetValue (Toke.GetValue ());
    std::cout << "Setting the value of variable ' v" << Which << " ' : " << TemToken.GetValue () << endl;
	SignVarVal = false; 
    return StartStatement;
    }

Statement::StatementStates Statement::GotEvalStatement (Statement &)
    {
	if (MT.ResetLeftCheck () == MathTree::Ok)
		{
		if (MT.GetResult (MathTreeResult) == MathTree::Bad)
				{
				ClearEverything ();
				std::cout << "You should build a new tree." << endl;
				}
		else
			{
			std::cout << "The result of the expression is: " << MathTreeResult << endl;
			std::cout << '\n';
			}
		}
		else;
    return StartStatement;
    }

Statement::StatementStates Statement::NotYetImplimented (Statement &)
    {
    cout << "This combination of statement is not yet implemented." << endl;
    return StartStatement;
    }

Statement::StatementStates Statement::GotEndOFFile (Statement & S)
    {
    std::cout << "****** Arriving to the end of the file ******" << endl;
	std::cout << '\n';
    return StatementComplete;
    }

Statement::StatementStates Statement::GotInvalidStatement (Statement & S)
    {
    std::cout << "Got an invalid statement." << endl;
    return StartStatement;
    }
