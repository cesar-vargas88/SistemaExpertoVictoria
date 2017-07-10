#include "StdAfx.h"
#include "InferenceEngine.h"
#include <algorithm>
#include <iterator>
#include <queue>

CInferenceEngine::CInferenceEngine(CPROPNode* pKnowledgeTree,CPROPCompiler* p_C)
{
	m_pKnowledgeTree=pKnowledgeTree;
	this->m_pBuilder=p_C;
	RuleCont = 1;
	
}
CInferenceEngine::~CInferenceEngine(void)
{
}
CPROPNode* CInferenceEngine::NextRule(CPROPNode* pCurrentRule)
{
	if(pCurrentRule->m_pParent->m_pSecond->m_Type==CPROPNode::AND)
		return pCurrentRule->m_pParent->m_pSecond->m_pFirst;

	if(pCurrentRule->m_pParent->m_pFirst->m_Type==CPROPNode::IF &&pCurrentRule->m_pParent->m_pSecond->m_Type==CPROPNode::IF)
	{
		if(pCurrentRule==pCurrentRule->m_pParent->m_pFirst)
			return pCurrentRule->m_pParent->m_pSecond;
		return NULL;
	}
	else if(pCurrentRule->m_pParent->m_pFirst->m_Type==CPROPNode::IF &&pCurrentRule->m_pParent->m_pSecond->m_Type!=CPROPNode::IF)
	{
			return pCurrentRule->m_pParent->m_pSecond;
	}
	return NULL;
	
}
CPROPNode* CInferenceEngine::GetFirstRule(void)
{
	if(this->m_pKnowledgeTree)
		return this->m_pKnowledgeTree->m_pFirst;
	return NULL;
}
CPROPNode* CInferenceEngine::GetAntecedents(CPROPNode* pCurrentRule)
{
	return pCurrentRule->m_pFirst;
}
CPROPNode* CInferenceEngine::GetConsecuents(CPROPNode* pCurrentRule)
{
	return pCurrentRule->m_pSecond;
}
bool CInferenceEngine::ValidateKnowledge(void)
{ 
	CPROPNode* pRule;

	pRule=GetFirstRule();

	while(pRule)
	{
		if(pRule->m_pParent->m_Type!=CPROPNode::AND) 
			return false;
		if(pRule->m_Type!=CPROPNode::IF) 
			{
				cout<<"Regla # "<<RuleCont<<" No es un cubo"<<endl;
			    return false;
			}
		
		CPROPNode* pActual=this->GetAntecedents(pRule); 
		
		if( VerifyCube(pActual,this->s_Antecedents)) 
			{
				cout<<"Los antecedentes de la Regla # "<<RuleCont<<" forman un cubo"<<endl;		 
			}
		else 
			{
				cout<<"Los antecedentes de la Regla # "<<RuleCont<<" no forman un cubo"<<endl;	 	
				return false;
			}

		pActual=this->GetConsecuents(pRule); // 5

		if( VerifyCube(pActual,this->s_Consecuents))
			{
				cout<<"Los consecuentes de la Regla # "<<RuleCont<<" forman un cubo"<<endl;		 
		
			}
		else 
			{
				cout<<"Los consecuentes de la Regla # "<<RuleCont<<" no forman un cubo"<<endl;	  
				return false;
			}

		pRule=NextRule(pRule);
		this->RuleCont ++;
		
	}
	return true;
}
bool CInferenceEngine:: VerifyCube(CPROPNode* pActual,set<unsigned long>& Separate)
{

	if(pActual->m_Type==CPROPNode::ATOM) 
	{
		Separate.insert(pActual->m_nAtomKey);
		return true;
	}
	else if(pActual->m_Type==CPROPNode::AND)
	{
		CPROPNode * p_antActual=GetAntecedents(pActual);
		
		if( VerifyCube(p_antActual,Separate))
		{
			CPROPNode * p_consActual=this->GetConsecuents(pActual);
			
			if(this-> VerifyCube(p_consActual,Separate))
			{
			   return true;
			}
			else return false;
		}
		else return false;

	}
	else
	{
	  return false;
	}

}
void CInferenceEngine::AllConclusions(set<unsigned long> s_Antecedents,set<unsigned long> s_Consecuents)
{

	cout<<endl<<endl;
	cout<<"Los cubos que forman parte de los antecedentes de alguna regla son:"<<endl;
	for(auto it=s_Antecedents.begin();it!=s_Antecedents.end();it++)
	{
		cout<<m_pBuilder->m_SymbolTable.find(*it)->second<<", ";
	}

	cout<<endl<<endl;
	cout<<"Los cubos que son consecuentes de alguna regla son:"<<endl;
	for(auto it=s_Consecuents.begin();it!=s_Consecuents.end();it++)
	{
		cout<<m_pBuilder->m_SymbolTable.find(*it)->second<<", ";
	}

	set_difference(s_Consecuents.begin(),s_Consecuents.end(),s_Antecedents.begin(),s_Antecedents.end(),inserter(s_Conclusions,(s_Conclusions.end())) );
    cout<<endl<<endl<<"Las conclusiones son:"<<endl;
	for(auto it=s_Conclusions.begin();it!=s_Conclusions.end();it++)
	{

		cout<<m_pBuilder->m_SymbolTable.find(*it)->second<<", ";
	}

	cout<<endl;
}
void CInferenceEngine::Sense(void)
{
	int bandera = 0;
	CPROPNode *pAntecedents=GetAntecedents(p_pCurrentRule);
	set<unsigned long> AtomSet;
	CollectAtoms(pAntecedents,AtomSet);
	for(auto it=AtomSet.begin();it!=AtomSet.end();it++)
	{
		if(m_mapRelevantKnowledge.find(*it)==m_mapRelevantKnowledge.end() && bandera!=1)	//si no halla el significado pon el átomo
		{
			string strAtomLabel;
			/*map<unsigned long, string>::iterator it_map = m_mapMeans.begin();
			while(it_map != m_mapMeans.cend())
			{
				cout<<"\n"<<it_map->first<<" "<<it_map->second;
				it_map++;
			}
			string cad = "audio";
			string mycadena = it_map->second;
			if(cad == mycadena)
				cout<<"si lo encontre";
			else
				cout<<"nel";*/
			auto itS=m_mapMeans.find(*it);
			if(itS!=m_mapMeans.end())
				strAtomLabel=itS->second;
			else
				strAtomLabel=m_pBuilder->m_SymbolTable.find(*it)->second;
	
			cout<<"Responde las siguientes preguntas segun sea el caso"<<endl;
			cout<<endl;
			cout<<"-"<<strAtomLabel<<"?"<<endl;
			cout<<"Teclea '0' si tu respuesta es no"<<endl;
			cout<<"Teclea '1' si tu respuesta es si"<<endl;
			cout<<"Teclea '2' si no deseas o no puedes responder a la pregunta"<<endl;
			char Resp;
			int bandera2=0;
			do
			{
			cin>>Resp; 
			switch(Resp)
			{
			case '0':
				{
				m_mapRelevantKnowledge.insert(
					make_pair(*it,false));
				bandera=1;
				bandera2=1;
				}
			break;
			case '1':
				{
				m_mapRelevantKnowledge.insert(
					make_pair(*it,true));
				bandera=0;
				bandera2=1;
				}
			break;
			case '2':
				{
				bandera = 0;
				bandera2=1;
				continue;
				}
			default:
				{
					system("cls");
					cout<<"Seleccionaste una opcion no valida porfavor teclea nuevamente la opcion que deseas"<<endl;
					cout<<"Teclea '0' si tu respuesta es no"<<endl;
					cout<<"Teclea '1' si tu respuesta es si"<<endl;
					cout<<"Teclea '2' si no deseas o no puedes responder a la pregunta"<<endl;
				}
			}
			}
			while(bandera2!=1);
		}
	}
}
void CInferenceEngine::Sense2(void)
{
	m_mapRelevantKnowledge.clear();
	string x;
	m_Atoms=m_pBuilder->m_SymbolTable;
	do
	{
	x=TrueAntecedents.top();
	map<unsigned long, string>::iterator it_map = m_Atoms.begin();
	while(it_map != m_Atoms.end())
	{
		string Str = it_map->second;
		if(Str==x)
		{
			int Atk;
			Atk = it_map->first;
			m_mapRelevantKnowledge.insert(make_pair(Atk,true));
		}
		it_map++;
	}
	TrueAntecedents.pop();
	}
	while(TrueAntecedents.size()>0);
}
void CInferenceEngine::CollectAtoms(CPROPNode* pTree,set<unsigned long>& AtomSet)
{
	if(pTree->m_Type==CPROPNode::ATOM)
		AtomSet.insert((unsigned long)pTree->m_pFirst);
	else if(pTree->m_Type==CPROPNode::NOT)
		CollectAtoms(pTree->m_pFirst,AtomSet);
	else
	{
		CollectAtoms(pTree->m_pFirst,AtomSet);
		CollectAtoms(pTree->m_pSecond,AtomSet);
	}
}
void CInferenceEngine::Inference(void)
{
	CPROPNode *pAntecedents=GetAntecedents(p_pCurrentRule);
	bool verify=true;
	//p_pCurrentRule->VerifyRule=true;
	set<unsigned long> AtomSet;
	CollectAtoms(pAntecedents,AtomSet);
	for(auto it=AtomSet.begin();it!=AtomSet.end();it++)
	{
		if(m_mapRelevantKnowledge.find(*it)==m_mapRelevantKnowledge.end())
		{
			verify=false;
			s_Rulespen.insert(p_pCurrentRule);
		}
		else
		{
			if(this->m_mapRelevantKnowledge.find(*it)->second==false)
			{
				verify=false;
				p_pCurrentRule->VerifyRule==false;
			}
		}
	}
	if(verify)
		{

				AtomSet.clear();
				CPROPNode* pConsecuents=GetConsecuents(p_pCurrentRule);
				CollectAtoms(pConsecuents,AtomSet);
				for(auto it=AtomSet.begin();it!=AtomSet.end();it++)
				{
					if(m_mapRelevantKnowledge.find(*it)!=m_mapRelevantKnowledge.end())
					{
						if(m_mapRelevantKnowledge.find(*it)->second==false)
						break;
					}
					else
					{
					m_mapRelevantKnowledge.insert(make_pair(*it,true));
					p_pCurrentRule->VerifyRule==false;
					}
				}

		}
	AtomSet.clear();
	//extraer todos los antecedentes de la regla actual
	//verificar que todos los antecedentes formen parte dewl conocimiento relevante
	// si todos los antecedentes son valuados como verdadero, entonces incorporar
	// el conocimiento relevante, todos los consecuentes valuados como verdadeross.
	//
	//en el caso de que un consecuente disparado por el caso anterior se contradise por
	//el conocimiento relevantre, se pueden tomar dos alternativas:
	// a)rechasar el razonamiento consecutivo, abortando el ciclo de razonamiento.
	// b)ignorar la regla actual que presenta un consecuente contradictorio
	// con el conocimiento relevante y continuar con otra regla consecutiva.
	//debe notificarse al usuario de esta situacion.
	// c) resolver la contradiccion con el usuario..

	//Entender antes de implementar cualquier cosa, la regla de inferencia MODUS PONENS
}
void CInferenceEngine::Conclude(void)
{
	for(auto it=s_Conclusions.begin();it!=s_Conclusions.end();it++)
	{
		if(m_mapRelevantKnowledge.find(*it)!=m_mapRelevantKnowledge.end())
		{
			this->s_FinalConclusions.insert(m_mapRelevantKnowledge.find(*it)->first);
		}
	}
	if(s_FinalConclusions.size()>0)
	{
	//	cout<<"Las conclusiones inferidas hasta el momento son:"<<endl;
	//	cout<<endl;
		for(auto it=s_FinalConclusions.begin();it!=s_FinalConclusions.end();it++)
		{
	//		cout<<"-"<<m_mapMeans.find(*it)->second<<endl;
			Conclud.push(m_mapMeans.find(*it)->second);
			cout<<endl;
		}
	}
	else if(s_FinalConclusions.size()==0)
	{
		cout<<"Con la informacion proporcionada no se ha podido concluir nada aun"<<endl;
	}
}
	/*durante el proceso de inferencia, se tiene registro de todas las reglas
	que han disparado consecuentes a poartir de antecedentes.
	a partir de este registro se pueden conocer las reglas de unico consecuente
	que forma parte como antecedente de otras reglas.
	A los cosecuentes que no forman parte de de un antecedente para otras reglas
	se les llama CONCLUSIONES.
	alternativas de conclusion:

	a)Detener el razonamiento, en cuanto se verifique la primer conclusion.
	b) detener el razonamiento en cuanto se verifiquen algunas conclusiones
	c) poder dar al cliente la oportunidar de continuar xcon el razonamiento
	hasta agotar la base de conocimientos.
	
}
*/
void CInferenceEngine::ForwardChaining(void)
{
	s_Rulespen.clear();
	p_pCurrentRule=GetFirstRule();

	while(p_pCurrentRule)
	{
		if(p_pCurrentRule->VerifyRule==true)
		{
			
			Sense();
			system("cls");
			Inference();
			Conclude();
			p_pCurrentRule=NextRule(p_pCurrentRule);
			//if(p_pCurrentRule==NULL)
			//break;
			}
		else
		{
			p_pCurrentRule=NextRule(p_pCurrentRule);
		}
	}
	if(s_Rulespen.size()>0)
	
		cout<<"Hay Reglas pendientes deseas inferirlas?"<<endl;
		cout<<"Teclea 1 para inferirlas"<<endl;
		cout<<"Teclea 2 para detener la inferencia"<<endl;
		char opc;
		cin>>opc;
		int bandera=0;
		do
		{
		switch(opc)
		{
		case '1':
			{
				ForwardChaining();
				bandera=1;
			}
			break;
		case '2':
			{
				cout<<"Gracias  el razonamiento ha terminado"<<endl;
				bandera=1;
			}
			break;
		default:
				{
				if(isdigit(opc))
				cout<<"Solo se permite responder con 0 o 1.!!"<<endl;
				if(isalpha(opc))
				cout<<"Solo se permite responder con 0 o 1.!!"<<endl;
				}
		}
		}
		while(bandera!=1);
	}
void CInferenceEngine::VictoriaChaining(void)
{
	Sense2();
	p_pCurrentRule=GetFirstRule();
	while(p_pCurrentRule)
	{
		Inference();
		p_pCurrentRule=NextRule(p_pCurrentRule);
	}
	Conclude();
}

void CInferenceEngine::BackwardChaining()
{
	int nCount=1;
	string Resp2="Si";
	unsigned long Resp1;
	queue<unsigned long> TestConclusions;
	set<CPROPNode*> pRules;

	cout<<" Listado de conclusiones "<<endl<<endl;
	cout<<" Para seleccionarla teclea su clave"<<endl;

	for(auto it=this->s_Conclusions.begin();it!=s_Conclusions.end();it++)
	{
		//cout<<nCount<<")"<<" Identificador: "<<m_pBuilder->m_SymbolTable.find(*it)->second<<endl;
		cout<<"Si"<<m_mapMeans.find(*it)->second<<"?"<<endl;
		cout<<"   Clave:"<<this->m_pBuilder->m_SymbolTable.find(*it)->first<<endl<<endl;
		nCount++;
	}
	
	do{		
	    cout<<"Que es lo que deseas saber ?"; cout<<endl<<endl;
		cin>>Resp1;
		TestConclusions.push(Resp1);
		cout<<endl<<endl<<"Deseas saber algo mas?"<<endl<<endl<<"Teclea el numero segun corresponda"<<endl;
		cout<<"1.- Si"<<endl;
		cout<<"2.- No"<<endl;
		cin>>Resp2;
		cout<<endl;
	}while(Resp2 !="2");

	
	while(TestConclusions.size()!=0)
	{
		SearchBackward(TestConclusions,pRules);
		TestConclusions.pop();
	}

	this->m_pBuilder->RulesStack;

	MakeRules(pRules);

	ForwardChaining();

	
}

void CInferenceEngine::SearchBackward(queue<unsigned long> & TestConclusions,set<CPROPNode*>& pRules)
{
	set<unsigned long> Consecuents,Antecedents;
	
	

	CPROPNode* pNode=this->GetFirstRule();

while(pNode)
{
	int a=0;
	pNode=this->GetConsecuents(pNode);
	this->CollectAtoms(pNode,Consecuents);

	for(auto it=Consecuents.begin();it!=Consecuents.end();it++)
	{
		
		unsigned long Id=TestConclusions.front();

		if(Id==*it)
		{
	       a=1;
		   pNode=pNode->m_pParent;
		   pRules.insert(pNode);
		   pNode=GetAntecedents(pNode);
		   CollectAtoms(pNode,Antecedents);
		  

		   for(auto it=Antecedents.begin();it!=Antecedents.end();it++)
		   {
			   TestConclusions.push(*it);
		   }

		}

	}
	pNode=pNode->m_pParent;
	//if(a==0)
	//{
	//	//pNode=pNode->m_pParent;
	//	pNode->b_State=false;
	//}

	pNode=this->NextRule(pNode);
	Antecedents.clear();
	Consecuents.clear();

}

	



}

void CInferenceEngine::MakeRules(set<CPROPNode*>& pRules)
{
	set<CPROPNode*> pRulesMark,s_Rules;
	stack<CPROPNode*> Respaldo;
	Respaldo=m_pBuilder->RulesStack;

	while(this->m_pBuilder->RulesStack.size()!=0)
	{
		s_Rules.insert(m_pBuilder->RulesStack.top());
		m_pBuilder->RulesStack.pop();
	}

	set_difference(s_Rules.begin(),s_Rules.end(),pRules.begin(),pRules.end(),inserter(pRulesMark,(pRulesMark.end())));

	for(auto it=pRulesMark.begin();it!=pRulesMark.end();it++)
	{
		(*it)->VerifyRule=false;
	}

	m_pBuilder->RulesStack=Respaldo;

}

void CInferenceEngine::TrueRules()
{
	
	stack<CPROPNode*> Respaldo;
	Respaldo=m_pBuilder->RulesStack;

	while(this->m_pBuilder->RulesStack.size()!=0)
	{
		CPROPNode* pNew=m_pBuilder->RulesStack.top();
		pNew->VerifyRule=true;
		m_pBuilder->RulesStack.pop();
	}

	m_pBuilder->RulesStack=Respaldo;

}

void CInferenceEngine::AssignRelevantKnowledgeAndMeans(fstream & input)
{
   char c;
   string strID;
   string Means;
   int nState=0;
   int a=0;

   while(!input.eof())
   {
		input.read(&c, 1);   
		a=0;
		switch(c)
		{	
			case '\n':
			{
				strID="";
				nState=0;
				Means="";
				a=0;
				continue;
			}
			break;
			// ESPACIO
			case ':':
			{
				if(strID.length()!=0)
				{				
					auto it=this->m_pBuilder->m_RegisteredSymbols.find(strID);
					if(it==m_pBuilder->m_RegisteredSymbols.end()) 
					{
						strID="";
						nState=0;
						Means="";
						continue;
					}
					else 
					{
						do
						{  
							input.read(&c,1);

							Means+=c;
						}
						while(c!='\n'&&!input.eof());
						this->m_mapMeans.insert(make_pair(it->second,Means));
					}
					strID="";
					Means="";
					nState=0;
				}
				continue;
			}
			break;
			default:	
			{
				switch(nState)
				{
					case 0: 
					{
						if(isalpha(c) || (c=='_'))
						{
							strID+=c;
							nState=1;
						}
						continue;
					}
					break;
					case 1:
					{
						if(isalpha(c) || (c=='_' || isdigit(c)))
						{
							strID+=c;	
						}
						continue;
					}
					break;
				}
			}
		}
	}
}
