#include "StdAfx.h"
#include "PROPCompiler.h"
#include "PROPNode.h"
#include <iostream>


using namespace std;



CPROPCompiler::CPROPCompiler(void)
{
	this->m_SyntacticTree=NULL;
}


CPROPCompiler::~CPROPCompiler(void)
{
}

bool CPROPCompiler::CompileRPN(fstream& input)
{
	cont1=0;
	cont2=1;
	string strID;
	static long s_lID=0;
	int nState=0;
	CStack Stack(this);
	while(1)
	{
		char c;
		input.read(&c,1);
		if(c=='\n')
		{
			cont1=0;
			cont2 ++;
		}
		else
			cont1 ++;
		if(input.eof())
		{
			if(strID.length()!=0)
			{
				auto it=m_RegisteredSymbols.find(strID);
				if(it==m_RegisteredSymbols.end())
				{
					m_RegisteredSymbols.insert(
						make_pair(strID,s_lID));
				m_SymbolTable.insert(
					make_pair(s_lID,strID));
				Stack.push(Atom(s_lID++));
				}
				else
					Stack.push(Atom(it->second));
				strID="";
				nState=0;
			}
			break;
		}
			
		switch(c)
		{
		case '\t':
		case '\n':
		case '\r':
		case ' ':
			if(strID.length()!=0)
			{
				auto it=m_RegisteredSymbols.find(strID);
				if(it==m_RegisteredSymbols.end())
				{
					m_RegisteredSymbols.insert(
						make_pair(strID,s_lID));
				m_SymbolTable.insert(
					make_pair(s_lID,strID));
				Stack.push(Atom(s_lID++));
				}
				else
					Stack.push(Atom(it->second));
				strID="";
				nState=0;
			}
			continue;
			break;
		case '&':
			{
			if(Stack.size()<2)
			{
				cout<<"Error de sintaxis: no hay suficientes atomos para realizar la operacion &"<<endl;
				return false;
			}
			CPROPNode* pSecond=Stack.top();
			Stack.pop();
			CPROPNode* pFirst=Stack.top();
			Stack.pop();
			Stack.push(And(pFirst,pSecond));
			}
			continue;
			break;
		case'|':
			{
				if(Stack.size()<2)
				{
				cout<<"Error de sintaxis: no hay suficientes atomos para realizar la operacion |"<<endl;
				return false;
				}
			CPROPNode* pSecond=Stack.top();
			Stack.pop();
			CPROPNode* pFirst=Stack.top();
			Stack.pop();
			Stack.push(Or(pFirst,pSecond));
			}
			continue;
			break;
			case'-':
			{
				input.read(&c,1);
				if(c=='\n')
				cont2 ++;
				else
				cont1 ++;
				if(input.eof())
				{
					if(strID.length()!=0)
					{
				auto it=m_RegisteredSymbols.find(strID);
				if(it==m_RegisteredSymbols.end())
				{
					m_RegisteredSymbols.insert(
						make_pair(strID,s_lID));
				m_SymbolTable.insert(
					make_pair(s_lID,strID));
				Stack.push(Atom(s_lID++));
				}
				else
					Stack.push(Atom(it->second));
				strID="";
				nState=0;
					}
				cout<<"Error de sintaxis: operador incompleto falta el caracter '>' "<<endl;
				return false;
				break;
				}
				else
					if(c=='>')
					{
				if(Stack.size()<2)
				{
				cout<<"Error de sintaxis: no hay suficientes atomos para realizar la operacion ->"<<endl;
				return false;
				}
			CPROPNode* pSecond=Stack.top();
			Stack.pop();
			CPROPNode* pFirst=Stack.top();
			Stack.pop();
			Stack.push(If(pFirst,pSecond));
					}
					else
					{
						cout<<"Error de sintaxis: operador incompleto falta el caracter '>'"<<endl; 
						return false;
					}
			}
			continue;
			break;
			case'<':
			{
				input.read(&c,1);
				if(c=='\n')
				cont2 ++;
				else
				cont1 ++;
				if(input.eof())
				{
					if(strID.length()!=0)
				{
				auto it=m_RegisteredSymbols.find(strID);
				if(it==m_RegisteredSymbols.end())
				{
					m_RegisteredSymbols.insert(
						make_pair(strID,s_lID));
				m_SymbolTable.insert(
					make_pair(s_lID,strID));
				Stack.push(Atom(s_lID++));
				}
				else
					Stack.push(Atom(it->second));
				strID="";
				nState=0;
			}
				cout<<"Error de sintaxis: operador incompleto falta el caracter '-'"<<endl;
				break;
				}
				if(c=='-')
				{
					input.read(&c,1);
					if(c=='\n')
					cont2 ++;
					else
					cont1 ++;
					if(input.eof())
					{
						if(strID.length()!=0)
			{
				auto it=m_RegisteredSymbols.find(strID);
				if(it==m_RegisteredSymbols.end())
				{
					m_RegisteredSymbols.insert(
						make_pair(strID,s_lID));
				m_SymbolTable.insert(
					make_pair(s_lID,strID));
				Stack.push(Atom(s_lID++));
				}
				else
					Stack.push(Atom(it->second));
				strID="";
				nState=0;
						}
						cout<<"Error de sintaxis: operador incompleto falta el caracter '>'"<<endl;
					break;
					}
					else
						if(c=='>')
						{
							if(Stack.size()<2)
							{
								cout<<"Error de sintaxis: No hay suficientes atomos para realizar la operacion <->"<<endl;
							return false;
							}
							CPROPNode* pSecond=Stack.top();
							Stack.pop();
							CPROPNode* pFirst=Stack.top();
							Stack.pop();
							Stack.push(Iif(pFirst,pSecond));
						}
						else
						{
							cout<<"Error de sintaxis: operador incompleto falta '>'"<<endl;
							return false;
						}
				}
				else
				{
					cout<<"Error de sintaxis: operador incompleto falta '->'"<<endl;
					return false;
				}
			}
			continue;
			break;
		case'!':
			{
				if(Stack.size()<1)
				{
				cout<<"Error de sintaxis: atomos insuficientes"<<endl;
				return false;
				}
			CPROPNode* pFirst=Stack.top();
			Stack.pop();
			Stack.push(Not(pFirst));
			}
			continue;
			break;

		default:
			switch(nState)
			{
			case 0: //alpha o _
			if(isalpha(c) || (c== '_'))
			{
				strID+=c;
				nState=1;
			continue;
			}
			cout<<"Operador desconocido o caracter no admitido, ups!!";
			break;
			case 1:
				if(isalpha(c) || (c=='_') || isdigit(c))
				{
					strID+=c;
					continue;
				}
				cout<<"Operador desconocido o caracter no admitido, ups!!";
				break;
		}

	}
		return false;
}
	if(Stack.size()==1)
	{
		m_SyntacticTree=Stack.top();
		Stack.pop();
		return true;
	}
	else
	{
		//eliminar los subarboles anotados en la pila
		cout<<"Error de sintaxis: hay mas de un arbol existente"<<endl;
		return false;
	}
}
CPROPNode* CPROPCompiler::CreateNode()
{
	CPROPNode* pNewNode=new CPROPNode;
	memset(pNewNode, 0,sizeof(CPROPNode));
	return pNewNode;
}

bool CPROPCompiler::CompileINF(fstream& input)
	{
	int pyccont = 0;
	stack <char> OPStack;
	int parcont=0;
	int opcont=0;
	cont1=0;
	cont2=1;
	string strID;
	static long s_lID=0;
	int nState=0;
	CStack Stack(this);
	while(1)
	{
		char c;
		input.read(&c,1);
		if(c=='\n')
		{
			cont1=0;
			cont2 ++;
		}
		else
			cont1 ++;
		if(input.eof())
		{
			if(strID.length()!=0)
			{
				auto it=m_RegisteredSymbols.find(strID);
				if(it==m_RegisteredSymbols.end())
				{
					m_RegisteredSymbols.insert(
						make_pair(strID,s_lID));
				m_SymbolTable.insert(
					make_pair(s_lID,strID));
				Stack.push(Atom(s_lID++));
				}
				else
					Stack.push(Atom(it->second));
				strID="";
				nState=0;
			}
			if(Stack.size() > 1)
			{
				RulesStack=Stack;
			for(int i = pyccont; i>1; i --)
			{
				CPROPNode* pSecond=Stack.top();
							Stack.pop();
							CPROPNode* pFirst=Stack.top();
							Stack.pop();
							Stack.push(And(pFirst,pSecond));
			}
			}
			else
			{
				cout<<"insuficientes reglas para realizar la operacion ';'"<<endl;
				return false;
			}
			break;
		}
			
		switch(c)
		{
		case '\t':
		case '\n':
		case '\r':
		case ' ':
			if(strID.length()!=0)
			{
				auto it=m_RegisteredSymbols.find(strID);
				if(it==m_RegisteredSymbols.end())
				{
					m_RegisteredSymbols.insert(
						make_pair(strID,s_lID));
				m_SymbolTable.insert(
					make_pair(s_lID,strID));
				Stack.push(Atom(s_lID++));
				}
				else
					Stack.push(Atom(it->second));
				strID="";
				nState=0;
			}
			continue;
			break;
		case ';':
			{
				if(Stack.size() > 0)
				{
					if(parcont==0 && opcont==0)
				pyccont ++;
				
					else
						cout<<"Revisar parentesis y operaciones"<<endl;
				}
			
				else
				{
					cout<<"Debe haber almenos una operacion realizada para escribir ';'"<<endl;
					return false;
				}
				continue;
			}
		
		case '&':
			{
				OPStack.push('&'); //nombre de la pila.push('cadena')
				opcont ++;
				continue;
			}
		case'|':
			{
				OPStack.push('|'); 
				opcont ++;
				continue;
			}
			case'-':
			{
				input.read(&c,1);
				if(c=='\n')
				cont2 ++;
				else
				cont1 ++;
				if(input.eof())
				{
					if(strID.length()!=0)
					{
				auto it=m_RegisteredSymbols.find(strID);
				if(it==m_RegisteredSymbols.end())
				{
					m_RegisteredSymbols.insert(
						make_pair(strID,s_lID));
				m_SymbolTable.insert(
					make_pair(s_lID,strID));
				Stack.push(Atom(s_lID++));
				}
				else
					Stack.push(Atom(it->second));
				strID="";
				nState=0;
					}
				cout<<"Error de sintaxis: operador incompleto falta el caracter '>' "<<endl;
				return false;
				break;
				}
				else
					if(c=='>')
					{
						OPStack.push('-');
						opcont ++;
						continue;
					}
					else
					{
						cout<<"Error de sintaxis: operador incompleto falta el caracter '>' "<<endl;
						return false;
					}
				continue;
			}
			case'<':
				{
				input.read(&c,1);
				if(c=='\n')
				cont2 ++;
				else
				cont1 ++;
				if(input.eof())
				{
					if(strID.length()!=0)
					{
				auto it=m_RegisteredSymbols.find(strID);
				if(it==m_RegisteredSymbols.end())
				{
					m_RegisteredSymbols.insert(
						make_pair(strID,s_lID));
				m_SymbolTable.insert(
					make_pair(s_lID,strID));
				Stack.push(Atom(s_lID++));
				}
				else
					Stack.push(Atom(it->second));
				strID="";
				nState=0;
					}
				cout<<"Error de sintaxis: operador incompleto falta el caracter '->'"<<endl;
				break;
				}
				if(c=='-')
				{
					input.read(&c,1);
					if(c=='\n')
					cont2 ++;
					else
					cont1 ++;
					if(input.eof())
					{
						if(strID.length()!=0)
			{
				auto it=m_RegisteredSymbols.find(strID);
				if(it==m_RegisteredSymbols.end())
				{
					m_RegisteredSymbols.insert(
						make_pair(strID,s_lID));
				m_SymbolTable.insert(
					make_pair(s_lID,strID));
				Stack.push(Atom(s_lID++));
				}
				else
					Stack.push(Atom(it->second));
				strID="";
				nState=0;
						}
						cout<<"Error de sintaxis: operador incompleto falta el caracter '>'"<<endl;
					break;
					}
					else
						if(c=='>')
						{
						OPStack.push('<');
						opcont ++;
					continue;
						}
						else
						{
							cout<<"Error de sintaxis: operador incompleto falta el caracter '>'"<<endl;
							return false;
						}
				}
				}
		case'!':
			{
				OPStack.push('!');
				opcont ++;
				continue;
			}
		case'(':
			{
				parcont ++;
				continue;
			}
			case')':
			{
				parcont --;
				if(strID.length()!=0)
				{
					auto it=m_RegisteredSymbols.find(strID);
					if(it==m_RegisteredSymbols.end())
					{
						m_RegisteredSymbols.insert(make_pair(strID,s_lID));
						m_SymbolTable.insert(make_pair(s_lID,strID));
						Stack.push(Atom(s_lID++));
					}
					else
						Stack.push(Atom(it->second));
					strID="";
					nState=0;
				}
				
				if(OPStack.size()>0)
				{
					switch(OPStack.top())
					{
						case '&':
						{
							if(Stack.size()<2)
							{
								cout << "Error de sintaxis: se necesitan al menos dos elementos para realizar la operacion & ";
								return false;
							}
							CPROPNode* pSecond=Stack.top();
							Stack.pop();
							CPROPNode* pFirst=Stack.top();
							Stack.pop();
							Stack.push(And(pFirst,pSecond));
							OPStack.pop();
							opcont --;
							break;
						}
						case '|':
						{
							if(Stack.size()<2)
							{
								cout << "Error de sintaxis: se necesitan al menos dos elementos para realizar la operacion |";
								return false;
							}
							CPROPNode* pSecond=Stack.top();
							Stack.pop();
							CPROPNode* pFirst=Stack.top();
							Stack.pop();
							Stack.push(Or(pFirst,pSecond));
							OPStack.pop();
							opcont --;
							break;
						}
						case '-':
						{
							if(Stack.size()<2)
							{
								cout << "Error de sintaxis: se necesitan al menos dos elementos para realizar la operacion ->";
								return false;
							}
							CPROPNode* pSecond=Stack.top();
							Stack.pop();
							CPROPNode* pFirst=Stack.top();
							Stack.pop();
							Stack.push(If(pFirst,pSecond));
							OPStack.pop();
							opcont --;
							break;
						}
						case '<':
						{
							if(Stack.size()<2)
							{
								cout << "Error de sintaxis: se necesitan al menos dos elementos para realizar la operacion <->";
								return false;
							}
							CPROPNode* pSecond=Stack.top();
							Stack.pop();
							CPROPNode* pFirst=Stack.top();
							Stack.pop();
							Stack.push(Iif(pFirst,pSecond));
							OPStack.pop();
							opcont --;
							break;
						}
						case '!':
						{
							if(Stack.size()<1)
							{
								cout <<"Error de sintaxis: se necesitan al menos dos elementos para realizar la operacion !";
								return false;
							}
							CPROPNode* pFirst=Stack.top();
							Stack.pop();
							Stack.push(Not(pFirst));
							OPStack.pop();
							opcont --;
							break;
						}
						default:
						{
							cout<<"\n\n\tError de sintaxis: Elementos insuficientes para realizar la operacion "<<OPStack.top()<<". ";
							return false;
						}
					}
				}	
				continue;
			}
			//}
		default:
/*			if(c==';')
			{

			}*/
			switch(nState)
			{
			case 0: //alpha o _
			if(isalpha(c) || (c== '_'))
			{
				strID+=c;
				nState=1;
			continue;
			}
			cout<<"Operador desconocido o caracter no admitido, ups!!";
			break;
			case 1:
				if(isalpha(c) || (c=='_') || isdigit(c))
				{
					strID+=c;
					continue;
				}
				cout<<"Operador desconocido o caracter no admitido, ups!!";
				break;
			}

	}
		return false;
}

//	if(Stack.size()==1)
	//{
	if(parcont==0 && opcont==0)
	{
		if(Stack.size()==1)
		{
		m_SyntacticTree=Stack.top();
		Stack.pop();
		return true;
		}
		else
			{
			//eliminar los subarboles anotados en la pila
			cout<<"Error de sintaxis: hay mas de un arbol existente"<<endl;
			return false;
			}
	}
	else
	{
		if(parcont < 0)
	{
		cout<<" Error de sintaxis: Tienes "<<parcont<<" ')' de mas"<<endl;
		return false;
	}
		if(parcont > 0)
	{
		cout<<" Error de sintaxis: Tienes "<<parcont<<" '(' sin cerrar"<<endl;
		return false;
	}
		if(opcont > 0)
	{
		cout<<" Error de sintaxis: Tienes "<<opcont<<" operaciones sin realizar"<<endl;
		return false;
		}
	}
	//else
/*	{
		//eliminar los subarboles anotados en la pila
		cout<<"Error de sintaxis: hay mas de un arbol existente"<<endl;
		return false;
		}*/
	}
	




void CPROPCompiler::DeleteTree(CPROPNode* pTree)
{
		if(pTree->m_Type==CPROPNode::ATOM)
		{
			delete pTree;
			return;
		}

		if(pTree->m_pFirst!=NULL)
			DeleteTree(pTree->m_pFirst);
		if(pTree->m_pSecond!=NULL)
			DeleteTree(pTree->m_pSecond);
		delete pTree;
	}
void CPROPCompiler::PrintTree(CPROPNode* pTree)
{
	switch(pTree->m_Type)
	{
	case CPROPNode::ATOM:
		cout<<m_SymbolTable.find(pTree->m_nAtomKey)->second;
		break;
	case CPROPNode::NOT:
		cout<<"!(";
		PrintTree(pTree->m_pFirst);
		cout<<")";
		break;

	case CPROPNode::AND:
		cout<<"(";
		PrintTree(pTree->m_pFirst);
		cout<<" & ";
		PrintTree(pTree->m_pSecond);
		cout<<")";
		break;

	case CPROPNode::OR:
		cout<<"(";
		PrintTree(pTree->m_pFirst);
		cout<<" | ";
		PrintTree(pTree->m_pSecond);
		cout<<")";
		break;

		case CPROPNode::IF:
		cout<<"(";
		PrintTree(pTree->m_pFirst);
		cout<<" -> ";
		PrintTree(pTree->m_pSecond);
		cout<<")";
		break;

	case CPROPNode::IIF:
		cout<<"(";
		PrintTree(pTree->m_pFirst);
		cout<<" <-> ";
		PrintTree(pTree->m_pSecond);
		cout<<")";
		break;
	}
}


CPROPNode* CPROPCompiler::Atom(int nAtomKey)
{
	CPROPNode* pAtom=CreateNode();
	pAtom->m_Type=CPROPNode::ATOM;
	pAtom->m_nAtomKey=nAtomKey;
	return pAtom;
}

void CPROPCompiler::SetParent(CPROPNode* pChild, CPROPNode* pParent)
{
	pChild->m_pParent=pParent;
}

CPROPNode* CPROPCompiler::And(CPROPNode* pFirst, CPROPNode* pSecond)
{
	CPROPNode* pProp=CreateNode();
	pProp->m_Type=CPROPNode::AND;
	pProp->m_pFirst=pFirst;
	pProp->m_pSecond=pSecond;
	SetParent(pFirst,pProp);
	SetParent(pSecond,pProp);
	return pProp;
}

CPROPNode* CPROPCompiler::Or(CPROPNode* pFirst, CPROPNode* pSecond)
{
	CPROPNode* pProp=CreateNode();
	pProp->m_Type=CPROPNode::OR;
	pProp->m_pFirst=pFirst;
	pProp->m_pSecond=pSecond;
	SetParent(pFirst,pProp);
	SetParent(pSecond,pProp);
	return pProp;
}

CPROPNode* CPROPCompiler::If(CPROPNode* pFirst, CPROPNode* pSecond)
{
	CPROPNode* pProp=CreateNode();
	pProp->m_Type=CPROPNode::IF;
	pProp->m_pFirst=pFirst;
	pProp->m_pSecond=pSecond;
	SetParent(pFirst,pProp);
	SetParent(pSecond,pProp);
	return pProp;
}

CPROPNode* CPROPCompiler::Iif(CPROPNode* pFirst, CPROPNode* pSecond)
{
	CPROPNode* pProp=CreateNode();
	pProp->m_Type=CPROPNode::IIF;
	pProp->m_pFirst=pFirst;
	pProp->m_pSecond=pSecond;
	SetParent(pFirst,pProp);
	SetParent(pSecond,pProp);
	return pProp;
}

CPROPNode* CPROPCompiler::Not(CPROPNode* pFirst)
{
	CPROPNode* pProp=CreateNode();
	pProp->m_Type=CPROPNode::NOT;
	pProp->m_pFirst=pFirst;
	SetParent(pFirst,pProp);
	return pProp;
}

bool CPROPCompiler::Evaluate(CPROPNode* pTree)
{
	switch(pTree->m_Type)
	{
	case CPROPNode::ATOM:
		{
			auto it=m_Valuation.find(pTree->m_nAtomKey);
			if(it!=m_Valuation.end())
				return it->second;
			return false;
		}
		break;
	case CPROPNode::AND:
		return Evaluate(pTree->m_pFirst) && Evaluate(pTree->m_pSecond);
	case CPROPNode::OR:
		return Evaluate(pTree->m_pFirst) || Evaluate(pTree->m_pSecond);
	case CPROPNode::NOT:
		return !Evaluate(pTree->m_pFirst);
		case CPROPNode::IF:
		return !Evaluate(pTree->m_pFirst) || Evaluate(pTree->m_pSecond);
		case CPROPNode::IIF:
		return (!Evaluate(pTree->m_pFirst) && !Evaluate(pTree->m_pSecond))||(Evaluate(pTree->m_pFirst) && Evaluate(pTree->m_pSecond));
		//agrega IF e IIF 
	}
}