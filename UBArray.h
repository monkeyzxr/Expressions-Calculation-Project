#ifndef UBARRAY_H
#define UBARRAY_H

#include <WCS_Pointer.h>
#include <iostream>

// NOTE:	for the following template, the requirement will be that whatever type
//			is used for the INDEX must have the comparison operators < <= == != >= >

using namespace std;

template <typename DATA, typename INDEX = int>
class UBArray
	{
	public:
	enum Exception {ArrayEmpty, NoNextElement, NoPrevElement, NoSuchELementExist, NoCurrentElement, ELementDontExist};
	private:
		struct Node : public RefCount 
			{
			DATA					data;
			INDEX					index;
			WCS_Pointer<Node>		PPrev;
			WCS_Pointer<Node>		PNext;
									Node    ();
									Node    (const INDEX &);
									~Node   ();
			};
   
	private:
					 WCS_Pointer <Node>		  PHead;
					 WCS_Pointer <Node>		  PTail;
			mutable  WCS_Pointer <Node>		  PPrev;
			mutable  WCS_Pointer <Node>		  PNext; 
			mutable  WCS_Pointer <Node>       PCurrent;
			mutable  WCS_Pointer <Node>		  PCurrent_for_data;
					 int                      NumElements;
											  UBArray              (const UBArray <DATA, INDEX> &);   
					 UBArray<DATA, INDEX> &	  operator =           (const UBArray <DATA, INDEX> &); 
	public:
											  UBArray              ();
											  ~UBArray             ();
					 DATA &					  operator []          (const INDEX &);
			const	 DATA &					  operator []          (const INDEX &) const;
					 DATA &					  AT                   (const INDEX &);
			const	 DATA &					  AT                   (const INDEX &) const;
			const	 INDEX &				  ShowIndex            (const WCS_Pointer <Node> &) const;
					 bool				      Remove               (const INDEX &);
					 bool					  RemoveAll            ();
			const	 int &					  Size                 () const;
					 WCS_Pointer <Node> &	  GetFirst             ();
			const	 WCS_Pointer <Node> &	  GetFirst             () const;
					 WCS_Pointer <Node> &	  GetLast              ();
			const	 WCS_Pointer <Node> &	  GetLast              () const;    
					 WCS_Pointer <Node> &	  GetNext              (const WCS_Pointer <Node> &);
			const	 WCS_Pointer <Node> &	  GetNext              (const WCS_Pointer <Node> &) const;
					 WCS_Pointer <Node> &	  GetPrev              (const WCS_Pointer <Node> &);
			const	 WCS_Pointer <Node> &	  GetPrev              (const WCS_Pointer <Node> &) const;
					 WCS_Pointer <Node> &	  GetCurrent           (const INDEX &);
			const	 WCS_Pointer <Node> &	  GetCurrent           (const INDEX &) const;
					 DATA &					  GetFirstData         ();  
			const	 DATA &					  GetFirstData         () const;
					 DATA &					  GetLastData          ();
			const	 DATA &					  GetLastData          () const;
					 DATA &					  GetNextData          (const INDEX &);
			const	 DATA &					  GetNextData          (const INDEX &) const;
					 DATA &					  GetPrevData          (const INDEX &);
			const	 DATA &					  GetPrevData          (const INDEX &)  const;
					 DATA &					  GetNext              ();       
			const    DATA &					  GetNext              () const; 
					 DATA &					  GetPrev              ();		 
			const    DATA &					  GetPrev              () const; 
					 void					  Print                ();
	private:
					 void					  SetAllIteratorToNull (const WCS_Pointer <Node> &);
					 void					  SetAllIteratorToNull ();
	};

template <typename DATA, typename INDEX>
UBArray <DATA, INDEX>::UBArray ()
	{
	NumElements = 0;
	}

template <typename DATA, typename INDEX>
UBArray <DATA, INDEX>::UBArray (const UBArray <DATA, INDEX> & Source_UBArray) 
	{
	if (!Source_UBArray.Size())
		NumElements = 0;
		else
			{
			NumElements = Source_UBArray.Size();
			PHead       = Source_UBArray.GetFirst();
			PTail       = Source_UBArray.GetLast();
			}
	}

template <typename DATA, typename INDEX>
UBArray <DATA, INDEX>::~UBArray ()				
	{
	int					i;
	if (!NumElements);
		else
			{
			for (i = 1; PHead != PTail; i++)
				{
				PHead = PHead.operator*().PNext;
				PHead.operator*().PPrev.SetToNull();
				cout << "Destroy " << i << " element" << endl;
				}
			PHead.SetToNull();
			PTail.SetToNull();
			SetAllIteratorToNull();
			cout << "Destroy " << i << " element" << endl;
			}
	}

template <typename DATA, typename INDEX>
UBArray <DATA, INDEX>::Node::Node () :index (0)
	{
	}

template <typename DATA, typename INDEX>
UBArray <DATA, INDEX>::Node::Node (const INDEX & i) :index (i)
	{
	}

template <typename DATA, typename INDEX>
UBArray <DATA, INDEX>::Node::~Node ()
	{
	}

template <typename DATA, typename INDEX>
inline const int & UBArray <DATA, INDEX>::Size () const
	{
	return NumElements;
	}

template <typename DATA, typename INDEX>
DATA & UBArray <DATA, INDEX>::operator [] (const INDEX & index)
	{
	WCS_Pointer <Node>	PTemp;
	if (PHead.operator!())       
		{
		PHead.New(Node(index)); 
		PTail = PHead;
		NumElements++;
		return PHead.operator*().data;
		}
		else if (index < PHead.operator*().index)//before head
			{
			PTemp.New(Node(index));
			PHead.operator*().PPrev = PTemp;
			PTemp.operator*().PNext = PHead;
			PHead = PTemp;
			NumElements++;
			return PHead.operator*().data;
			}
			else if (index == PHead.operator*().index)//At the head
				return PHead.operator*().data;
				else if (index == PTail.operator*().index) //At the end
					return PTail.operator*().data;
					else if (index > PTail.operator*().index)//After the end
						{
						PTemp.New(Node(index));
						PTail.operator*().PNext = PTemp;
						PTemp.operator*().PPrev = PTail;
						PTail = PTemp;
						NumElements++;
						return PTail.operator*().data;
						}
						else
							{
							for (PTemp = PHead.operator*().PNext; index > PTemp.operator*().index; PTemp = PTemp.operator*().PNext);
							if (PTemp.operator*().index == index)
								return PTemp.operator*().data;
								else
									{
									WCS_Pointer<Node> PTemp1, PTemp2;
									PTemp1 = PTemp.operator*().PPrev;
									PTemp2.New(Node(index));
									PTemp.operator*().PPrev = PTemp2;
									PTemp2.operator*().PNext = PTemp;
									PTemp2.operator*().PPrev = PTemp1;
									PTemp1.operator*().PNext = PTemp2;
									NumElements++;
									return PTemp2.operator*().data;
									}
							}
	}  

template <typename DATA, typename INDEX>
inline const DATA &	UBArray <DATA, INDEX>::operator [] (const INDEX & index) const
	{
	int					i;
	WCS_Pointer <Node>	PTemp;
	if (PHead.operator!())
		throw ArrayEmpty;
		else if ((index<PHead.operator*().index) || (index>PTail.operator*().index))
			throw NoSuchELementExist;
			else
				{
				for (i=1, PTemp = PHead; i <= NumElements; i++, PTemp = PTemp.operator*().PNext)
					{
					if (index == PTemp.operator*().index)
						return PTemp.operator*().data;
						else;
					}
				throw NoSuchELementExist;
				}
	}

template <typename DATA, typename INDEX>
inline DATA & UBArray <DATA, INDEX>::AT (const INDEX & index)
	{
	return UBArray <DATA, INDEX>::operator [] (index);
	}

template <typename DATA, typename INDEX>
inline const DATA & UBArray <DATA, INDEX>::AT (const INDEX & index) const
	{
	return UBArray <DATA, INDEX>::operator [] (index);
	}

template <typename DATA, typename INDEX>
inline const INDEX & UBArray <DATA, INDEX>::ShowIndex (const WCS_Pointer <Node> & Pnow) const
	{
	if (Pnow.operator!())
		throw ELementDontExist;
		else
			return Pnow.operator*().index;
	}

template <typename DATA, typename INDEX>
bool UBArray <DATA, INDEX>::Remove (const INDEX & index)
	{
	int					i;
	WCS_Pointer <Node>	PTemp;
	if (PHead.operator!())
		return false;
		else if ((index<PHead.operator*().index) || (index>PTail.operator*().index))
			 return false;
			else if (index == PHead.operator*().index)
				{
				SetAllIteratorToNull(PHead);
				if (NumElements == 1)
					{
					PHead.SetToNull();
					PTail.SetToNull();
					}
				else
					{
					PHead = PHead.operator*().PNext;
					PHead.operator*().PPrev.SetToNull();
					}
				NumElements--;
				return true;
				}
				else if (index == PTail.operator*().index)
					{
					SetAllIteratorToNull(PTail);
					PTail = PTail.operator*().PPrev;
					PTail.operator*().PNext.SetToNull();
					NumElements--;
					return true;
					}
					else 
						{
						for (i = 1, PTemp = PHead.operator*().PNext; i <= NumElements - 2; i++)
							{
							if (index == PTemp.operator*().index)
								{
								WCS_Pointer <Node> PTemp1, PTemp2;
								PTemp1 = PTemp.operator*().PPrev;
								PTemp2 = PTemp.operator*().PNext;
								SetAllIteratorToNull(PTemp);
								PTemp1.operator*().PNext = PTemp2;
								PTemp2.operator*().PPrev = PTemp1;
								NumElements--;
								return true;
								}
							else
								PTemp = PTemp.operator*().PNext;
							}
						return false;
						}
	}

template <typename DATA, typename INDEX>
bool UBArray <DATA, INDEX>::RemoveAll ()
	{
	NumElements = 0;
	for (;PHead != PTail;)
		{
		PHead = PHead.operator*().PNext;
		PHead.operator*().PPrev.SetToNull();
		}
	PHead.SetToNull();
	PTail.SetToNull();
	SetAllIteratorToNull();
	return true;
	} 

template <typename DATA, typename INDEX>
inline WCS_Pointer <typename UBArray <DATA, INDEX>::Node> & UBArray <DATA, INDEX>::GetFirst () 
	{
	if (PHead.operator!())
		throw ArrayEmpty; 
		else
			return PHead;
	}

template <typename DATA, typename INDEX>
inline const WCS_Pointer <typename UBArray <DATA, INDEX>::Node> & UBArray <DATA, INDEX>::GetFirst () const
	{
	if (PHead.operator!())
		throw ArrayEmpty;
		else
			return PHead;
	}

template <typename DATA, typename INDEX>
inline WCS_Pointer <typename UBArray <DATA, INDEX>::Node> & UBArray <DATA, INDEX>::GetLast ()
	{
	if (PHead.operator!())
		throw ArrayEmpty; 
		else
			return PTail;
	}

template <typename DATA, typename INDEX>
inline const WCS_Pointer <typename UBArray <DATA, INDEX>::Node> & UBArray <DATA, INDEX>::GetLast () const
	{
	if (PHead.operator!())
		throw ArrayEmpty;
		else
			return PTail;
	}
template <typename DATA, typename INDEX>
inline WCS_Pointer <typename UBArray <DATA, INDEX>::Node> &  UBArray <DATA, INDEX>::GetNext (const WCS_Pointer <Node> & PNow)
	{
	PNext = PNow.operator*().PNext;  
	if (PHead.operator!())
		throw ArrayEmpty; 
		else if (PNow.operator!())
			throw NoCurrentElement; 
			else if (PNext.operator!())
				throw NoNextElement; 
				else
					return PNext;
	}

template <typename DATA, typename INDEX>
inline const WCS_Pointer <typename UBArray <DATA, INDEX>::Node> & UBArray <DATA, INDEX>::GetNext (const WCS_Pointer <Node> & PNow) const
	{
	PNext = PNow.operator*().PNext;
	if (PHead.operator!())
		throw ArrayEmpty; 
			else if (PNow.operator!())
				throw NoCurrentElement; 
				else if (PNext.operator!())
					throw NoNextElement; 
					else  
						return PNext;
	}

template <typename DATA, typename INDEX>
inline WCS_Pointer <typename UBArray <DATA, INDEX>::Node> &	UBArray <DATA, INDEX>::GetPrev (const WCS_Pointer <Node> & PNow)
	{
	PPrev = PNow.operator*().PPrev; 
	if (PHead.operator!())
		throw ArrayEmpty; 
		else if (PNow.operator!())
			throw NoCurrentElement; 
			else if (PPrev.operator!())
					throw NoPrevElement; 
					else
						return PPrev;
	}

template <typename DATA, typename INDEX>
inline const WCS_Pointer <typename UBArray <DATA, INDEX>::Node> & UBArray <DATA, INDEX>::GetPrev (const WCS_Pointer <Node> & PNow) const
	{
	PPrev = PNow.operator*().PPrev; 
	if (PHead.operator!())
		throw ArrayEmpty;
		else if (PNow.operator!())
			throw NoCurrentElement; 
			else if (PPrev.operator!())
				throw NoPrevElement; 
				else
					return PPrev;
	}

template <typename DATA, typename INDEX>
WCS_Pointer <typename UBArray <DATA, INDEX>::Node> &  UBArray <DATA, INDEX>::GetCurrent (const INDEX & index)
	{
	WCS_Pointer <Node>   PTemp;
	if (PHead.operator!())
		throw ArrayEmpty;
		else if ((index<PHead.operator*().index) || (index > PTail.operator*().index))
			throw NoCurrentElement;
			else 
				{
				for (PTemp = PHead; PTemp != PTail; PTemp = PTemp.operator*().PNext)
					{
					if (index == PTemp.operator*().index)
						{
						PCurrent = PTemp;
						return PCurrent;
						}
						else;
					}
				if (index == PTail.operator*().index)
					{
					PCurrent = PTail;
					return PCurrent;
					}
					else
						throw NoCurrentElement;
				}
	}

template <typename DATA, typename INDEX>
const WCS_Pointer <typename UBArray <DATA, INDEX>::Node> &	UBArray <DATA, INDEX>::GetCurrent (const INDEX & index) const
	{
	WCS_Pointer <Node>   PTemp;
	if (PHead.operator!())
		throw ArrayEmpty;
	else if ((index<PHead.operator*().index) || (index > PTail.operator*().index))
		throw NoCurrentElement;
	else 
		{
		for (PTemp = PHead; PTemp != PTail; PTemp = PTemp.operator*().PNext)
			{
			if (index == PTemp.operator*().index)
				{
				PCurrent = PTemp;
				return PCurrent;
				}
			else;
			}
		if (index == PTail.operator*().index)
			{
			PCurrent = PTail;
			return PCurrent;
			}
		else
			throw NoCurrentElement;
		}
	}

template <typename DATA, typename INDEX>
inline DATA & UBArray <DATA, INDEX>::GetFirstData ()
	{
	if (PHead.operator!())
		throw ArrayEmpty;
		else
			{
			PCurrent_for_data=PHead;
			return PHead.operator*().data;
			}

	}

template <typename DATA, typename INDEX>
inline const DATA &	UBArray <DATA, INDEX>::GetFirstData () const
	{
	if (PHead.operator!())
		throw ArrayEmpty;
		else
			{
			PCurrent_for_data=PHead;
			return PHead.operator*().data;
			}
	}

template <typename DATA, typename INDEX>
inline  DATA & UBArray <DATA, INDEX>::GetLastData ()
	{
	if (PHead.operator!())
		throw ArrayEmpty;
		else
			{
			PCurrent_for_data=PTail;
			return PTail.operator*().data;
			}
	}

template <typename DATA, typename INDEX>
inline const DATA &	UBArray <DATA, INDEX>::GetLastData ()  const
	{
	if (PHead.operator!())
		throw ArrayEmpty;
		else
			{
			PCurrent_for_data=PTail;
			return PTail.operator*().data;
			}
	}

template <typename DATA, typename INDEX>
DATA &	UBArray <DATA, INDEX>::GetNextData (const INDEX & index)
	{
	GetCurrent(index);
	return GetNext(PCurrent).operator*().data;
	}

template <typename DATA, typename INDEX>
const DATA & UBArray <DATA, INDEX>::GetNextData (const INDEX & index)  const
	{
	GetCurrent(index);
	return GetNext(PCurrent).operator*().data;
	}

template <typename DATA, typename INDEX>
DATA & UBArray <DATA, INDEX>::GetPrevData (const INDEX & index)
	{
	GetCurrent(index);
	return GetPrev(PCurrent).operator*().data;
	}

template <typename DATA, typename INDEX>
const DATA & UBArray <DATA, INDEX>::GetPrevData (const INDEX &)  const
	{
	GetCurrent(index);
	return GetPrev(PCurrent).operator*().data;
	}

template <typename DATA, typename INDEX>
DATA & UBArray <DATA, INDEX>::GetNext ()
	{
	if(PCurrent_for_data.operator!())
		 throw NoCurrentElement;
		else
			{
			PCurrent_for_data=GetNext(PCurrent_for_data);
			return PCurrent_for_data.operator*().data;
			}
	}

template <typename DATA, typename INDEX>
const DATA & UBArray <DATA, INDEX>::GetNext () const
	{
	if (PCurrent_for_data.operator!())
		 throw NoCurrentElement;
		else
			{
			PCurrent_for_data = GetNext(PCurrent_for_data);
			return PCurrent_for_data.operator*().data;
			}
	}

template <typename DATA, typename INDEX>
DATA & UBArray <DATA, INDEX>::GetPrev ()
	{
	if (PCurrent_for_data.operator!())
		 throw NoCurrentElement;
		else
			{
			PCurrent_for_data = GetPrev(PCurrent_for_data);
			return PCurrent_for_data.operator*().data;
			}
	}

template <typename DATA, typename INDEX>
const DATA & UBArray<DATA, INDEX>::GetPrev () const
	{
	if (PCurrent_for_data.operator!())
		 throw NoCurrentElement;
		else
			{
			PCurrent_for_data = GetPrev(PCurrent_for_data);
			return PCurrent_for_data.operator*().data;
			}
	}

template <typename DATA, typename INDEX>
void UBArray <DATA, INDEX>::SetAllIteratorToNull (const WCS_Pointer <Node> & P)
	{
	if (PPrev == P)
		 PPrev.SetToNull();
	else;
	if (PNext == P)
		 PNext.SetToNull();
	else;
	if (PCurrent == P)
		 PCurrent.SetToNull();
	else;
	if (PCurrent_for_data == P)
		 PCurrent_for_data.SetToNull();
	else;
	}

template <typename DATA, typename INDEX>
void UBArray <DATA, INDEX>::SetAllIteratorToNull ()
	{
	PPrev.SetToNull();
	PNext.SetToNull();
	PCurrent.SetToNull();
	PCurrent_for_data.SetToNull();
	}

template <typename DATA, typename INDEX>
void UBArray <DATA, INDEX>::Print ()
	{
	WCS_Pointer <Node>  PTemp;
	for (PTemp = PHead; PTemp != PTail; PTemp = (*PTemp).PNext)
		cout << "[ " << ShowIndex(PTemp) <<" ] "<<"------"<< (*this)[ShowIndex(PTemp)] << endl;
	cout << "[ " << ShowIndex(PTemp) << " ] " << "------" << (*this)[ShowIndex(PTemp)] << endl;
	}

#endif

