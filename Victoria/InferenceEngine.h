#pragma once
#include <string>
#include "PROPCompiler.h"
#include "PROPNode.h"
#include <queue>


class CInferenceEngine
{
public:
	string m_strOut;
	string m_srtIn;
	stack <string> Conclud;
	stack <string> TrueAntecedents;
public:
	CInferenceEngine(void);
	~CInferenceEngine(void);

	//ORDEN DE PROCESAMIENTO
	virtual void Sense();// N° 1
	virtual void Sense2();
	virtual void Inference(); //N° 2
	virtual void Conclude(); // N° 3

public:

	CInferenceEngine(CPROPNode* pKnowledgeTree,CPROPCompiler* C);
	CPROPNode* m_pKnowledgeTree;
	CPROPCompiler* m_pBuilder;
	CPROPNode* p_pCurrentRule; 
	map<unsigned long, bool> m_mapRelevantKnowledge;
	map<unsigned long, string> m_mapMeans;
	int RuleCont;
	set<unsigned long> s_Antecedents;
	set<unsigned long> s_Consecuents;
	set<unsigned long> s_Conclusions;
	set<unsigned long> s_FinalConclusions;
	set<CPROPNode*> s_Rulespen;

	CPROPNode* NextRule(CPROPNode* pCurrentRule);
	CPROPNode* GetFirstRule(void);
	CPROPNode* GetAntecedents(CPROPNode* pCurrentRule);
	CPROPNode* GetConsecuents(CPROPNode* pCurrentRule);
	void ForwardChaining(void);
	void BackwardChaining(void);
	void VictoriaChaining(void);
	void AssignRelevantKnowledgeAndMeans(fstream & input);
	void SearchBackward(queue<unsigned long> & TestConclusions,set<CPROPNode*>& pRules);
	void MakeRules(set<CPROPNode*>& pRules);
	void TrueRules();
	void CollectAtoms(CPROPNode* pTree, set<unsigned long>& AtomSet); //Atomset= Conjunto de atomos
	bool ValidateKnowledge(void);
	bool VerifyCube(CPROPNode* Actual,set<unsigned long>& Separate);
	void AllConclusions(set<unsigned long> s_Antecedents,set<unsigned long> s_Consecuents);

	map<unsigned long,string> m_Atoms;
// esto es solo una prueba
	

};
