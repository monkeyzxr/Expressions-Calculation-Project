#ifndef STATEMENT_H
#define STATEMENT_H

#include <WCS_Pointer.h>
#include <WCS_String.h>
#include "MathTree.h"
#include "Token.h"

class Statement
	{
	private:
		enum	StatementStates		{
									StatementComplete = -1,
									StartStatement,
                                    AfterExpToken,
                                    SkipToSemicolon,
                                    AfterOperandToken,
                                    AfterOperatorToken,                                                            
                                    AfterOpenPareToken,
                                    AfterValueToken,
                                    AfterVarName,
                                    AfterVarSign,
                                    AfterVarValue,
                                    AfterEvalToken,

									NumStates
									};

    private:
		void	Build	             (const char*);
    private:
		//		**********	typedefs	**********
        typedef StatementStates      (*StateFunc)               (Statement &);
		//	    **********	functions	**********
        static  StatementStates      NotYetImplimented          (Statement &);
        static  StatementStates      GotEndOFFile               (Statement &);
        static  StatementStates      GotInvalidStatement        (Statement &); 
        static  StatementStates      GotEvalToken               (Statement &);
        static  StatementStates      GotExpToken                (Statement &);
        static  StatementStates      GotValueToken              (Statement &);
        static  StatementStates      GotOperandToken            (Statement &);
        static  StatementStates      GotSignToken               (Statement &);
        static  StatementStates      GotOpenPareToken           (Statement &); 
        static  StatementStates      GotEndofStatement          (Statement &);
        static  StatementStates      GoToSkipToSemicolon        (Statement &); 
        static  StatementStates      ContinueSkipToSemicolon    (Statement &); 
        static  StatementStates      GotOperatorToken           (Statement &); 
        static  StatementStates      GotCloseParenToken         (Statement &); 
        static  StatementStates      GotExpStatement            (Statement &); 
        static  StatementStates      GotUnexpectedSemicolon     (Statement &);
        static  StatementStates      GotVarName                 (Statement &);
        static  StatementStates      GotVarSign                 (Statement &);
        static  StatementStates      GotVarValue                (Statement &); 
        static  StatementStates      GotVarValStetement         (Statement &); 
        static  StatementStates      GotEvalStatement           (Statement &);
        static  StateFunc            StateFunctions    [];
    private:
        static  void                 ClearEverything   ();
    private:
        static  int                  NumParen;
        static  int                  Which;
        static  bool                 PlusSign;
        static  Variable::ValueType  MathTreeResult; 
		static  bool                 SignVarVal;
    private:
		static	Token		         Toke;
		static	MathTree	         MT;
		static	int			         StateTable [Token::NumTokens] [NumStates];
    public:
                                     Statement        ();
                                     ~Statement       ();
		void                         ReadStatement    (const char*);
    };


inline Statement::Statement ()
    {
	}

inline Statement::~Statement ()
    {
	}

inline void Statement::ClearEverything ()
    {
    MT.RemoveAllNodes();
    NumParen = 0;
    }

inline void Statement::ReadStatement (const char* P)
    {
	(*this).Build(P);
    }

#endif
