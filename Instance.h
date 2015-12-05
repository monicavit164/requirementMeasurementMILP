#ifndef __INSTANCE__
#define __INSTANCE__
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <list>
#include <map>


using namespace std;


class Instance{
private:
	int NMetriche;
	int NSiti;
	int NAttivita;

	double Budget;

	double** CostoValutazione;
	double** CostoSonda;
	double*** Precisione;
	double*** Delta1Precisione;
	double*** Delta2Precisione;

	vector <int> ElencoAttivitaNoStesso;
	vector <int> ElencoAttivitaStesso;
	
	vector <double> CostoFisso;
	vector<double> GradimentoMetriche;

	map<int, vector<int>>  Metrics;
	map<int, vector<int>>  SpecialRequest;
	map<int, vector<int>>  MetricsRequest;

public:
	Instance(string pathTogether, string pathNoTogether,
			string pathSpecialRequest, string pathEvaluationCost,
			string pathFixedCost, string pathProbeCost, 
			string pathPrecision, string pathDelta1Precision,
			string pathDelta2Precision, string pathBudget,
			string pathMM,string pathMR);

	virtual ~Instance() {}
	
	int getNMetriche();
	int getNAttivita();
	int getNSiti();

	double getBudget();
	double getPrecisione(int k, int j, int i);
	double getDeltaZPrecisione(int k, int j, int i);
	double getDeltaYPrecisione(int k, int j, int i);
	double getPrice(int j);
	double getEstimationPrice(int k,int j);
	double getProbePrice(int k,int j);
	double getPreferenze(int k);
	double getMinAccuracy();

	void Stampa();
	void duplicaAttivit‡(int a1, int a2, int nCopie);
	void setGradimentoMetriche(vector<double> Gradimento);
	void consideraAttivit‡From(int to);


	vector<int> getAttivitaStesso();
	vector<int> getAttivitaNoStesso();

	map<int, vector<int>>  getSpecialRequest();
	map<int, vector<int>>  getMetrics(); // restituisce l'albero delle metric measurements
	map<int, vector<int>>  getMetricsRequest();
};

#endif

