#pragma once
class CPROPNode
{
public:
	
		
	enum NodeType{ATOM,AND,OR,IF,IIF,NOT};

	NodeType m_Type;
	CPROPNode* m_pParent;
	union
	{
	CPROPNode* m_pFirst;
	int m_nAtomKey; //si y solo si el nodo es tipo ATOM
	};
	CPROPNode* m_pSecond;

	CPROPNode();
	~CPROPNode();
	bool VerifyRule;
};

