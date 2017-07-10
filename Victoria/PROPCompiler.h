#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <set>
#include <stack>

using namespace std;

class CPROPNode;
class CPROPCompiler
{
public: int cont1, cont2;
protected:
	class CStack:public stack<CPROPNode*>
	{
	public:
		CPROPCompiler* m_pBackOwner;
		CStack(CPROPCompiler* pOwner){m_pBackOwner=pOwner;};
		~CStack()
		{
			while(size())
			{
				m_pBackOwner->DeleteTree(top());
				pop();
			}
		};
	};
public:
	//almacen de todos los simbolos correspondientes a los atomos
	map<unsigned long,string> m_SymbolTable;
	map<string, int> m_RegisteredSymbols;
	stack<CPROPNode*> RulesStack;
	CPROPNode* m_SyntacticTree;

	map<int,bool> m_Valuation;
	bool Evaluate(CPROPNode* pTree);

	CPROPCompiler(void);
	~CPROPCompiler(void);

	bool CompileRPN(fstream& input);
	bool CompileINF(fstream& input);
	bool CompileExamen(fstream& input);
	void DeleteTree(CPROPNode* pTree);
	void PrintTree(CPROPNode* pTree); //realiza la traduccion del arbol sintactico a su forma infija

	CPROPNode* CreateNode();
	CPROPNode* Atom(int nAtomKey);
	void SetParent(CPROPNode* pChild, CPROPNode* pParent);
	CPROPNode* And(CPROPNode* pFirst, CPROPNode* pSecond);
	CPROPNode* Or(CPROPNode* pFirst,CPROPNode* pSecond);
	CPROPNode* If(CPROPNode* pFirst, CPROPNode* pSecond);
	CPROPNode* Iif(CPROPNode* pFirst, CPROPNode* pSecond);
	CPROPNode* Not(CPROPNode* pFirst);
};
