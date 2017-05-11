#include "MathTree.h"
#include "TokenTypePrint.h"
#include <iomanip>

MathTree::Node::Node (const Token & Toke) :Token (Toke) 
	{
	LeftChecked = false;
	LeftDeleted = false;
    }

MathTree::Node::Node (Token::TokenType TokType, int Which) :Token (TokType, Which)
    {
    LeftChecked = false;
    LeftDeleted = false;
    }

MathTree::Node::Node (Token::TokenType TokType, Variable::ValueType Val) :Token (TokType, Val) 
	{
	LeftChecked = false;
	LeftDeleted = false;
	}

MathTree::MathTree ()
	{
    NumNodes             = 0;
    VarDefineState_Index = 0;
	}

MathTree::~MathTree ()
	{
	RemoveAllNodes();
	}

MathTree::ResultCode MathTree::RemoveAllNodes ()
	{
	WCS_Pointer <Node>	PNode;
	if (!PRoot.Exists())
		 return Ok;
		else;
	PNode = PRoot;
	while (PNode.Exists())
		{
		if ((*PNode).LeftDeleted == false)
			{
			while ((*PNode).PLeftChild.Exists())
				{
				PNode				 = GetTheMostLeft(PNode);    
				PNode			     = (*PNode).PParent;
				(*PNode).PLeftChild.SetToNull();
				(*PNode).LeftDeleted = true;
                if ((*PNode).PRightChild.Exists())
				    PNode			= (*PNode).PRightChild; 
                else;
				}
			PNode = (*PNode).PParent;
			}
        else if (PNode.Exists())
			{
			(*PNode).PRightChild.SetToNull();
			PNode = (*PNode).PParent;
			if (PNode.Exists())
				{
				if ((*PNode).LeftDeleted == false)  //Get back from the left
					{
					(*PNode).PLeftChild.SetToNull();
					(*PNode).LeftDeleted = true;
					PNode = (*PNode).PRightChild;
					}
					else; //Otherwise get back from the right
				}
				else;
			}
        else;
		}
	PRoot.SetToNull();
	NumNodes = 0;
    PLastOperatorNode.SetToNull();
    for (int i = 0; i < VarDefineState.Size(); i++)
        Token::VarClearDefine(VarDefineState[i]);
    VarDefineState.RemoveAll();
    VarDefineState_Index = 0;
	return Ok;
	}

MathTree::ResultCode MathTree::InsertOperand (const Token & Tok)
	{
	WCS_Pointer <Node>    PNode(new Node(Tok));
	if (!PRoot.Exists())
           PRoot = PNode;
	    else
		    {
            if (((*PRoot).GetType() == Token::ConstantToken) || ((*PRoot).GetType() == Token::VariableToken))
                    {
                    cout << "You should enter an operator before inputting an operand."<< endl;
                    return Bad;
                    }
                else 
                    if ((*PLastOperatorNode).PRightChild.Exists())
                    {
                    cout << "You should enter an operator before inputting an operand." << endl;
                    return Bad;
                    }
                    else;
		    (*PLastOperatorNode).PRightChild = PNode;
		    (*PNode).PParent				 = PLastOperatorNode;
		    }
    if ((*PNode).GetType() == Token::VariableToken)
            {
			bool AlreadyCount(false);
			for (int i = 0; i < VarDefineState.Size(); i++)
				{
					if (VarDefineState[i] == (*PNode).GetWhich())
							{
                            AlreadyCount = true;
								break;
							}
						else;
				}
            if (!AlreadyCount)
					{
						VarDefineState[VarDefineState_Index] = (*PNode).GetWhich();
						VarDefineState_Index++;
					}
				else;
            }
        else;
	NumNodes++;
	return Ok;
	}

MathTree::ResultCode MathTree::InsertBinaryOperator (const Token & Tok)
	{												  
	WCS_Pointer <Node>		PNode(new Node(Tok));
    if (!PRoot.Exists())
        {
        if (Tok.GetType() == Token::OperatorMinusToken)
                {
                Token TemToken(Tok);
                TemToken.SetPrecedence(Operator::UnaryPlusMinusPrecedence);
                InsertUniaryOperator(TemToken);
                return Ok;
                }
                else 
                   if (Tok.GetType() == Token::OperatorPlusToken)
                    {
                    Token TemToken(Tok);
                    TemToken.SetPrecedence(Operator::UnaryPlusMinusPrecedence);
                    InsertUniaryOperator(TemToken);
                    return Ok;
                    }
                   else
                       {
                       cout << "You should enter an operand before inputting an operator." << endl;
                       return Bad;
                       }
        }
		else;   
        while (PLastOperatorNode.Exists() && !HigherPrecedence(PNode, PLastOperatorNode))
            {
            if (!(*PLastOperatorNode).PRightChild.Exists())
                    {
                    cout <<"You should enter an operand before inputting an operator."<< endl;
                    return Bad;
                    }
                else;
            PLastOperatorNode = (*PLastOperatorNode).PParent;
            }
	if (!PLastOperatorNode.Exists()) 
		{
		(*PNode).PLeftChild = PRoot;
		(*PRoot).PParent	= PNode;
		PRoot				= PNode;
		PLastOperatorNode	= PRoot;
		}
	else      
		{
        if (!(*PLastOperatorNode).PRightChild.Exists())
                {
                cout << "You should enter an operand before inputting an operator." << endl;
                return Bad;
                }
            else;
		(*PNode).PLeftChild					= (*PLastOperatorNode).PRightChild;
		(*(*PNode).PLeftChild).PParent		= PNode;
		(*PLastOperatorNode).PRightChild	= PNode;
		(*PNode).PParent					= PLastOperatorNode;
		PLastOperatorNode					= PNode;
		}
	NumNodes++;
	return Ok; 
	}

MathTree::ResultCode MathTree::InsertUniaryOperator (const Token & Tok)
	{
	if (PLastOperatorNode.Exists() && ((*PLastOperatorNode).GetPrecedence() == Operator::UnaryPlusMinusPrecedence))
			{
				if ((*PLastOperatorNode).GetType() == Token::OperatorPlusToken)
					{
						if (Tok.GetType() == Token::OperatorPlusToken);
							else
								(*PLastOperatorNode).SetType(Token::OperatorMinusToken);			
					}
					else
					{
						if (Tok.GetType() == Token::OperatorPlusToken);
							else
								(*PLastOperatorNode).SetType(Token::OperatorPlusToken);
					}
			}
		else
		{
			InsertOperand(Token(Token::ConstantToken, 0));
			InsertBinaryOperator(Tok);
		}
	return Ok;
	}

MathTree::ResultCode MathTree::GetResult (Variable::ValueType & Result)
	{
	WCS_Pointer <Node>	PNode;
	Variable::ValueType	PTemResult;
    int j(0);
    if (!PRoot.Exists())
        {
        cout << "This is an empty tree." << endl;
        return Ok;
        }
        else 
            if (IsOperand(PRoot))
                {
                 Result = (*PRoot).GetValue();
                 return Ok;
                }
		        else;
    for (int i = 0; i < VarDefineState.Size(); i++)
        {
        if (!(Token::VarDefined(VarDefineState[i])))
            {
            cout << "Variable V" << VarDefineState[i] << "'s Value is not defined." << endl;
            j++;
            }
        else;
        }
    if (j>0)
            {
            cout << "You should initiate vairables before evaluation." << endl;
            return Ok;
            }
        else;
	PNode = PRoot;
    if ((*PNode).LeftChecked == true)
            {
            Result = (*PRoot).GetValue();
            return Ok;
            }
        else;
	while (PNode.Exists())
		{
		if ((*PNode).LeftChecked == false)
			    {
                while ((*PNode).PLeftChild.Exists())  
				    {
				    PNode = GetTheMostLeft(PNode);
				    PNode = (*PNode).PParent;
				    (*PNode).LeftChecked = true;
				    (*PNode).SetValue((*(*PNode).PLeftChild).GetValue());
                    if ((*PNode).PRightChild.Exists())
                        PNode = (*PNode).PRightChild;
                    else
                        {
                        cout << "The MathTree is not completed." << endl;
                        return Bad;
                        }
				    }
			    PNode = (*PNode).PParent;
			    }
		    else    
			    {            
                if (CalculTwoNodes((*PNode).GetValue(), (*(*PNode).PRightChild).GetValue(), PNode, PTemResult) == Bad)
						{
						return Bad;
						}
					else;
			    (*PNode).SetValue(PTemResult);
			    PNode = (*PNode).PParent;
			    if (PNode.Exists())
				        {
				        if ((*PNode).LeftChecked == false)
					           {
					            (*PNode).LeftChecked = true;
					            (*PNode).SetValue((*(*PNode).PLeftChild).GetValue());
                                if ((*PNode).PRightChild.Exists())
                                        PNode = (*PNode).PRightChild;
                                    else
                                        {
                                        cout << "The MathTree is not completed." << endl;
                                        return Bad;
                                        }
					           }
				            else;
			           }
			         else;
			    }
		}
	Result = (*PRoot).GetValue();
	return Ok;
	}

MathTree::ResultCode MathTree::ResetLeftCheck ()
    {
    WCS_Pointer <Node> PNode;
    if (!PRoot.Exists())
            return Ok;
        else;
    PNode = PRoot;
    while (PNode.Exists())
        {
        if ((*PNode).LeftChecked == true)
            {
            while ((*PNode).PLeftChild.Exists())  
                {
                PNode = GetTheMostLeft(PNode);
                PNode = (*PNode).PParent;
                (*PNode).LeftChecked = false;
                if ((*PNode).PRightChild.Exists())
                        PNode = (*PNode).PRightChild;
                    else
                        {
                        cout << "The MathTree is not completed. The current tree is being destroyed." << endl;
                        RemoveAllNodes();
                        cout << "You need to build a new tree." << endl;
                        return Bad;
                        }
                }
            PNode = (*PNode).PParent;
            }
        else  
            {
            PNode = (*PNode).PParent;
            if (PNode.Exists())
                    {
                    if ((*PNode).LeftChecked == true) 
                            {
                            (*PNode).LeftChecked = false;
                            if ((*PNode).PRightChild.Exists())
                                    PNode = (*PNode).PRightChild;
                                else
                                    {
                                    cout << "The MathTree is not completed. The current tree is being destroyed." << endl;
                                    RemoveAllNodes();
                                    cout << "You need to build a new tree." << endl;
                                    return Bad;
                                    }
                            }
                        else;
                    }
                else;
            }
        }
    return Ok;
    }

 WCS_Pointer <MathTree::Node> & MathTree::GetPLastOper () 
	{
	return PLastOperatorNode;
	}

 const WCS_Pointer <MathTree::Node> & MathTree::GetPLastOper () const
	{
	return PLastOperatorNode;
	}

const WCS_Pointer <MathTree::Node> & MathTree::GetPRoot () const
	{
	return PRoot;
	}

const WCS_Pointer <MathTree::Node> MathTree::GetTheMostLeft (const WCS_Pointer <Node> & PNode) const   
	{
	WCS_Pointer <Node>  PTem;
	PTem = PNode;
	while ((*PTem).PLeftChild.Exists())
		PTem = (*PTem).PLeftChild;
	return PTem;
	}

bool MathTree::IsOperand (const WCS_Pointer <Node> & PNode) const
	{
	if (PNode.Exists())
		return (((*PNode).GetType() == Token::VariableToken) || ((*PNode).GetType() == Token::ConstantToken));
	else
		return false;
	}

bool MathTree::IsOperator (const Token & Tok) const
	{
	bool	Result(false);
	switch (Tok.GetType())
		{
		case Token::OperatorPlusToken:
		case Token::OperatorMinusToken:
		case Token::OperatorAsteriskToken:
		case Token::OperatorSlashToken:
			Result = true;
			break;
		default:
			Result = false; 
		}
	return Result;
	}

bool MathTree::IsOperator (const WCS_Pointer <Node> & PNode)const
	{
	return IsOperator((*PNode));
	}

bool MathTree::IsBinaryOperator (const WCS_Pointer <Node> & PNode) const
	{
	return ((IsOperand(GetPRoot()) || (*GetPLastOper()).PRightChild.Exists()) && (IsOperator(PNode)));
	}

bool MathTree::IsBinaryOperator () const
	{
	if (!PLastOperatorNode.Exists())
		return IsOperand(GetPRoot());
	else
		return (*PLastOperatorNode).PRightChild.Exists();
	}

MathTree::ResultCode  MathTree::CalculTwoNodes (
												Variable::ValueType		 VL, 
												Variable::ValueType		 VR, 
												const WCS_Pointer<Node>& Opera,
												Variable::ValueType&	 Result) const
	{
	if (!IsOperand(Opera))
		{
		switch ((*Opera).GetType())
			{
			case Token::OperatorMinusToken:
				Result = VL - VR;
				break;
			case Token::OperatorPlusToken:
				Result = VL + VR;
				break;
			case Token::OperatorAsteriskToken:
				Result = VL * VR;
				break;
			case Token::OperatorSlashToken:
                if (VR == 0)
                    {
                    cout << "Error! Divided by Zero. The current tree is being destroyed..." << endl;
                    return Bad;
                    }
				else
					Result = VL / VR;
			default:;
			}
		return Ok;
		}
	else
		return Bad;
	}

MathTree::ResultCode  MathTree::CalculTwoNodes (const WCS_Pointer<Node>&   NodeL,
											    const WCS_Pointer<Node>&   NodeR, 
											    const WCS_Pointer<Node>&   Opera, 
												      Variable::ValueType& Result) const
	{
	if (IsOperand(NodeR) && IsOperand(NodeL) && !IsOperand(Opera))
		{
		Variable::ValueType VL;
		Variable::ValueType VR;
		VL = (*NodeL).GetValue();
		VR = (*NodeR).GetValue();
		CalculTwoNodes(VL, VR, Opera, Result);
		return Ok;
		}
		else
			return Bad;
	}

bool MathTree::HigherPrecedence (const WCS_Pointer <Node> & PHigher, const WCS_Pointer <Node> & PLower) const
    {
    return (((*PHigher).GetPrecedence() + Operator::ParenthesesPrecedence * (*PHigher).GetNumIncludingParen()) > ((*PLower).GetPrecedence() + Operator::ParenthesesPrecedence * (*PLower).GetNumIncludingParen()));
    }