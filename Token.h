#ifndef TOKEN_H
#define TOKEN_H

#include <fstream>
#include <iostream>

using namespace std;

#include <WCS_String.h>
#include "Operator.h"
#include "Variable.h"
#include "RefCount.h"

class Token:public RefCount
	{
	public:
		//		**********	enums		**********
			enum	Variables				{
											NumVariables = 100
											};
			enum	Exception				{
											TokenFileNotOpen
											};
			enum	TokenType				{
											EndOfInputToken,
											UnknownToken,
											VariableToken,
											ConstantToken,
											KeywordEvalToken,
											KeywordExpToken,
											KeywordValueToken,
											OperatorPlusToken,
											OperatorMinusToken,
											OperatorAsteriskToken,
											OperatorSlashToken,
											SymbolOpenParenToken,
											SymbolCloseParenToken,
											SymbolSemiColonToken,

											NumTokens
											};
		//		**********	typedefs	**********
			typedef unsigned char			UChar;    
		//		**********	functions	**********
											Token			      ();
					explicit				Token			      (const Token &);
					explicit				Token			      (TokenType, int);
					explicit				Token			      (TokenType, Variable::ValueType);
											~Token			      ();
					Token &					Build			      ();
			static	void					CloseFile		      ();
			Operator::Precedence			GetPrecedence	      () const;
					TokenType				GetType			      () const;
					void					SetType			      (TokenType);
					Variable::ValueType		GetValue		      () const;
					int						GetWhich		      () const;
			static	void					OpenFile		      (const WCS_String &);
            static  bool                    VarDefined            (int);
            static  void                    VarClearDefine        (int);
					void					SetPrecedence	      (Operator::Precedence);
                    int                     GetNumIncludingParen  () const;
                    void                    SetNumIncludingParen  (int);
			        void					SetValue		      (Variable::ValueType);
					Token &					operator=		      (const Token &);
	private:
		//		**********	enums		**********
			enum	InputType				{
											InputUnknown,
											InputA,
											InputE,
											InputL,
											InputP,
											InputU,
											InputV,
											InputX,
											Input0,
											Input1,
											Input2,
											Input3,
											Input4,
											Input5,
											Input6,
											Input7,
											Input8,
											Input9,
											InputPlus,
											InputMinus,
											InputAsterisk,
											InputSlash,
											InputOpenParen,
											InputCloseParen,
											InputSemiColon,
											InputSpace,
											EndOfFile,

											NumInputs
											};
			enum	TokenState				{
											TokenComplete = -1,
											StartGettingToken,
											AfterE,
											AfterV,
											AfterEV,
											AfterEVA,
											AfterVA,
											AfterVAL,
											AfterVALU,
											AfterEX,
											AfterVarFirstDig,
											AfterConFirstDig,

											NumStates
											};
		//		**********	typedefs	**********
			typedef int						State;
			typedef TokenState				(*TokenFunc)		(Token &);
		//	**********	functions	**********
					InputType				GetInputChar			();
					void					Init					(TokenType, int);
					void					Init					(TokenType, Variable::ValueType);
					void					PutBackLastChar			() const;
					void					SetWhich				(int);
			static	TokenState				NotYetWritten			(Token &);
			static	TokenState				GotE					(Token &);
			static	TokenState				GotEV					(Token &);
			static	TokenState				GotEVA					(Token &);
			static	TokenState				GotEVAL					(Token &);
			static	TokenState				GotV					(Token &);
			static	TokenState				GotEndOfFile			(Token &);
			static	TokenState				GotUnexpectedEOF		(Token &);
			static	TokenState				StartedTokenWithSpace	(Token &);
			static	TokenState				GotUnknownInput			(Token &);
			static  TokenState				GotVA					(Token &);
			static  TokenState				GotVAL					(Token &);
			static  TokenState				GotVALU					(Token &);
			static  TokenState				GotVALUE				(Token &);
			static  TokenState				GotEX					(Token &);
			static  TokenState				GotEXP					(Token &);
			static	TokenState				GotVarFirstDig			(Token &);  
			static	TokenState				GotVariable				(Token &);
			static  TokenState				GotConFirstDig			(Token &);
			static  TokenState              GotConDig				(Token &);
			static  TokenState				GotCon					(Token &);
			static  TokenState				GotPlus					(Token &);
			static  TokenState				GotMinus				(Token &);
			static  TokenState				GotAsterisk				(Token &);
			static  TokenState				GotSlash				(Token &);
			static  TokenState              GotOpenParen			(Token &);
			static  TokenState              GotCloseParen			(Token &);
			static  TokenState				GotSemiColon			(Token &);

			static	InputType				InputTable		[];
			static	UChar					LastChar;
			static	ifstream				SourceFile;
			static	TokenFunc				StateFunction	[];
			static	UChar					StateTable		[NumInputs] [NumStates];
			static	Variable				VariableArray	[NumVariables];
		//	**********	data		**********
					TokenType				Type;
					union
					{
					int						Which;
					Operator::Precedence	Precedence;
					Variable::ValueType		Value;
					};
                    int                     NumIncludingParen;
	};

inline Token::Token ()
	{
	Init (EndOfInputToken, 0);
    NumIncludingParen = 0;
	}

inline Token::Token (const Token & T)
	{
	if (T.GetType() == Token::VariableToken)
		Init(T.GetType(), T.Which);
	else
		Init(T.GetType(), T.Value);
    NumIncludingParen = T.NumIncludingParen;
	}

inline Token::Token (TokenType NewType, int Which)
	{
	Init(NewType, Which);
    NumIncludingParen = 0;
	}

inline Token::Token(TokenType Type, Variable::ValueType V)
	{
	Init(Type, V);
    NumIncludingParen = 0;
	}

inline Token::~Token ()
	{
	}

inline void Token::CloseFile ()
	{
	SourceFile.close ();
	}

inline Operator::Precedence Token::GetPrecedence () const
	{
	return Precedence;
	}

inline Token::TokenType Token::GetType () const
	{
	return Type;
	}

inline Variable::ValueType Token::GetValue () const
	{
	return (Type == VariableToken) ? VariableArray [Which].GetValue () : Value;
	}

inline int Token::GetWhich () const
	{
	return Which;
	}

inline void Token::OpenFile (const WCS_String & FileName)
	{
	SourceFile.open (static_cast <const char *> (FileName));
	if (!SourceFile.is_open ())
			{
			cout << "File did not open" << endl;
			exit (0);
			}
		else;
	}

inline void Token::SetValue (Variable::ValueType V)
	{
	if (Type == VariableToken)
			VariableArray [Which].SetValue (V);
		else
			Value = V;
	}

inline Token & Token::operator= (const Token & T)
	{
	if (this != &T)
			Init (T.Type, T.Which);
		else;
	return *this;
	}

inline Token::InputType Token::GetInputChar ()
	{
	return InputTable [LastChar = static_cast <UChar> (SourceFile.get ())];
	}

inline void Token::Init (TokenType T, int V) 
	{
	Type  = T;
	if (T == Token::VariableToken)
		Which = V;
	else
		Value = V;
	}

inline void	 Token::Init(TokenType T, Variable::ValueType V)
	{
	Type  = T;
	Value = V;
	}

inline void Token::PutBackLastChar () const
	{
	SourceFile.putback (LastChar);
	}

inline void Token::SetPrecedence (Operator::Precedence P)
	{
	Precedence = P;
	}

inline void Token::SetType (TokenType T)
	{
	Type = T;
	}

inline void Token::SetWhich (int W)
	{
	Which = W;
	}

inline bool  Token::VarDefined(int Which)
    {
    return VariableArray[Which].IsDefined();
    }

inline void  Token::VarClearDefine(int Which)
    {
    VariableArray[Which].ClearDefined();
    }

#endif