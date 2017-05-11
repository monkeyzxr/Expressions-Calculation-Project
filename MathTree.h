#ifndef MATHTREE_H
#define MATHTREE_H

#include "Token.h"
#include "RefCount.h"
#include "UBArray.h"
#include <WCS_Pointer.h>

class MathTree
	{
	private:
		struct Node: public Token    
			{
											Node                   (const Token &);
                                            Node                   (Token::TokenType, int);
										    Node                   (Token::TokenType, Variable::ValueType);
			WCS_Pointer <Node>				PParent;
			WCS_Pointer <Node>				PLeftChild;
			WCS_Pointer <Node>				PRightChild;
			bool							LeftChecked;
			bool							LeftDeleted;
			};
	private:
			WCS_Pointer <Node>				PRoot;
			WCS_Pointer <Node>				PLastOperatorNode;
			int								NumNodes;
    private:
            UBArray <int>                   VarDefineState;
            int                             VarDefineState_Index;
	private:
			MathTree &						operator=               (const MathTree &);
											MathTree                (const MathTree &);
	public:
			enum							ResultCode              {Ok, Bad, NumResults};
			string							ResultArray             [NumResults];
	public:
											MathTree                ();
											~MathTree               ();
			ResultCode						InsertOperand           (const Token &);
			ResultCode						InsertBinaryOperator    (const Token &);
			ResultCode						InsertUniaryOperator    (const Token &);
			ResultCode						GetResult               (Variable::ValueType &);
            ResultCode                      ResetLeftCheck          ();
			ResultCode						CalculTwoNodes          (const WCS_Pointer <Node> &, const WCS_Pointer <Node> &, const WCS_Pointer <Node> &, Variable::ValueType &) const;
			ResultCode						CalculTwoNodes          (Variable::ValueType, Variable::ValueType, const WCS_Pointer <Node> &, Variable::ValueType &) const;
			ResultCode						RemoveAllNodes          ();
			ResultCode						ShowReusltCode          () const;
			bool							IsOperand               (const WCS_Pointer <Node> &) const;
			bool							IsBinaryOperator        (const WCS_Pointer <Node> &) const;
			bool							IsBinaryOperator        () const;
			bool							IsOperator              (const WCS_Pointer <Node> &)const;
			bool							IsOperator              (const Token &) const;
            bool                            HigherPrecedence        (const WCS_Pointer <Node> &, const WCS_Pointer <Node> &) const;
	private:
	       WCS_Pointer <Node> &				GetPLastOper            ();
	 const WCS_Pointer <Node> &             GetPLastOper            () const;
	 const WCS_Pointer <Node> &			    GetPRoot                () const;
	 const WCS_Pointer <Node>				GetTheMostLeft          (const WCS_Pointer <Node> &) const;
	};

#endif