#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <ctime>
#include <algorithm>
#include "instance.h"



using namespace std;

Instance::Instance(string pathTogether, string pathNoTogether,
			string pathSpecialRequest, string pathEvaluationCost,
			string pathFixedCost, string pathProbeCost, 
			string pathPrecision, string pathDelta1Precision,
			string pathDelta2Precision, string pathBudget, 
			string pathMM,string pathMR){


	string line;
	string word;

// Load Budget and Number of activity
	ifstream iffBNA(pathBudget.c_str());
	
	if (!iffBNA.is_open()){
		cout <<"Impossibile aprire il file"<<pathBudget<<endl;
		cin.get();
		exit(1);
	}
	
	
	
	while(getline (iffBNA,line)){
		std::replace(line.begin(), line.end(), ';', ' ');
		istringstream iss(line);
		iss>>word;
		Budget=atof(word.c_str());
		iss>>word;
		NAttivita=atoi(word.c_str());
	}

	iffBNA.close();


// Load File Together
	ifstream iffToghether(pathTogether.c_str());
	
	if (!iffToghether.is_open()){
		cout <<"Impossibile aprire il file"<<pathTogether<<endl;
		cin.get();
		exit(1);
	}

	while(getline (iffToghether,line)){
		std::replace(line.begin(), line.end(), ';', ' ');
		istringstream iss(line);
		iss >> word;
		ElencoAttivitaStesso.push_back(atoi(word.c_str()));
		iss >> word;
		ElencoAttivitaStesso.push_back(atoi(word.c_str()));
	}

	iffToghether.close();


// Load File NoTogether
	ifstream iffNoToghether(pathNoTogether.c_str());
	
	if (!iffNoToghether.is_open()){
		cout <<"Impossibile aprire il file"<<pathNoTogether<<endl;
		cin.get();
		exit(1);
	}

	while(getline (iffNoToghether,line)){
		std::replace(line.begin(), line.end(), ';', ' ');
		istringstream iss(line);
		iss >> word;
		ElencoAttivitaNoStesso.push_back(atoi(word.c_str()));
		iss >> word;
		ElencoAttivitaNoStesso.push_back(atoi(word.c_str()));
	}

	iffToghether.close();

// Load File SpecialRequest
	ifstream iffSR(pathSpecialRequest.c_str());
	
	if (!iffSR.is_open()){
		cout <<"Impossibile aprire il file"<<pathSpecialRequest<<endl;
		cin.get();
		exit(1);
	}

	while(getline (iffSR,line)){
		std::replace(line.begin(), line.end(), ';', ' ');
		istringstream iss(line);
		iss >> word;
		int attivita=atoi(word.c_str());
		
		while(iss >> word){
			SpecialRequest[attivita].push_back(atoi(word.c_str()));
		}
	}

	iffSR.close();

// Load File FixedCost
	ifstream iffFC(pathFixedCost.c_str());
	
	if (!iffFC.is_open()){
		cout <<"Impossibile aprire il file"<<pathFixedCost<<endl;
		cin.get();
		exit(1);
	}


	
	while(getline(iffFC,line)){
		istringstream iss(line);
		iss >> word;
		CostoFisso.push_back(atof(word.c_str()));
	}
		
	iffFC.close();
	

// Load File ProbeCosts

	ifstream iffPC(pathProbeCost.c_str());
	
	if (!iffPC.is_open()){
		cout <<"Impossibile aprire il file"<<pathProbeCost<<endl;
		cin.get();
		exit(1);
	}
	
	getline (iffPC,line);
	std::replace(line.begin(), line.end(), ';', ' ');
	istringstream iss(line);
	iss >> word;
	NMetriche = atoi(word.c_str());
	iss >> word;
	NSiti = atoi(word.c_str());
	
	CostoSonda = new double*[NMetriche];
	for(int k=0; k < NMetriche; k++){
		CostoSonda[k] = new double[NSiti];
	}

	for(int k=0; k < NMetriche; k++){
		getline (iffPC,line);
		std::replace(line.begin(), line.end(), ';', ' ');
		istringstream iss(line);
		for(int j=0; j < NSiti; j++){
			iss >> word;
			CostoSonda[k][j]=atof(word.c_str());
		}
	}
	
	iffPC.close();

// Load File EvaluationCosts

	ifstream iffEC(pathEvaluationCost.c_str());
	
	if (!iffEC.is_open()){
		cout <<"Impossibile aprire il file"<< pathEvaluationCost<<endl;
		cin.get();
		exit(1);
	}
	
	getline (iffEC,line);
	
	CostoValutazione = new double*[NMetriche];
	for(int k=0; k < NMetriche; k++){
		CostoValutazione[k] = new double[NSiti];
	}

	for(int k=0; k < NMetriche; k++){
		getline (iffEC,line);
		std::replace(line.begin(), line.end(), ';', ' ');
		istringstream iss(line);
		for(int j=0; j < NSiti; j++){
			iss >> word;
			CostoValutazione[k][j]=atof(word.c_str());
		}
	}
	
	iffEC.close();



// Load File Precison
	ifstream iffP(pathPrecision.c_str());
	
	if (!iffP.is_open()){
		cout <<"Impossibile aprire il file"<<pathPrecision<<endl;
		cin.get();
		exit(1);
	}
	
	
	Precisione = new double**[NMetriche];
	for(int k = 0; k < NMetriche; k++){
			Precisione[k] = new double*[NSiti];
			for(int j = 0; j < NSiti; j++){
				Precisione[k][j] = new double[NAttivita];
			}
	}
	
	getline (iffP,line);
	for(int j=0; j < NSiti; j++){
		for(int k=0; k < NMetriche; k++){
			getline (iffP,line);
			std::replace(line.begin(), line.end(), ';', ' ');
			istringstream iss(line);
			for(int i=0; i<NAttivita; i++){
				iss >> word;
				Precisione[k][j][i] = atof(word.c_str());
			}
		}
	}
	iffP.close();

// Load FileDeltaPrecision
	ifstream iffD1P(pathDelta1Precision.c_str());
	
	if (!iffD1P.is_open()){
		cout <<"Impossibile aprire il file"<<pathDelta1Precision<<endl;
		cin.get();
		exit(1);
	}
	

	Delta1Precisione = new double**[NMetriche];
	for(int k = 0; k < NMetriche; k++){
			Delta1Precisione[k] = new double*[NSiti];
			for(int j = 0; j < NSiti; j++){
				Delta1Precisione[k][j] = new double[NAttivita];
			}
	}
	
	getline (iffD1P,line);
	for(int j=0; j < NSiti; j++){
		for(int k = 0; k < NMetriche; k++){
			getline (iffD1P,line);
			std::replace(line.begin(), line.end(), ';', ' ');
			istringstream iss(line);
			for(int i = 0; i<NAttivita; i++){
				iss >> word;
				Delta1Precisione[k][j][i]=atof(word.c_str());

			}
		}
	}

	iffD1P.close();

// Load File Delta2Precision
	ifstream iffD2P(pathDelta2Precision.c_str());
	
	if (!iffD2P.is_open()){
		cout <<"Impossibile aprire il file"<<pathDelta2Precision<<endl;
		cin.get();
		exit(1);
	}
	
	Delta2Precisione = new double**[NMetriche];
	for(int k = 0; k < NMetriche; k++){
			Delta2Precisione[k] = new double*[NSiti];
			for(int j = 0; j < NSiti; j++){
				Delta2Precisione[k][j] = new double[NAttivita];
			}
	}
	
	getline (iffD2P,line);
	for(int j=0; j < NSiti; j++){
		for(int k=0; k < NMetriche; k++){
			getline (iffD2P,line);
			std::replace(line.begin(), line.end(), ';', ' ');
			istringstream iss(line);
			for(int i=0; i< NAttivita;i++){
				iss >> word;
				Delta2Precisione[k][j][i]=atof(word.c_str());
			}
		}
	}
	iffD2P.close();

// Load File MetricMeasurements
	ifstream iffMM(pathMM.c_str());
	
	if (!iffMM.is_open()){
		cout <<"Impossibile aprire il file"<<pathMM<<endl;
		cin.get();
		exit(1);
	}

	int count=0;
	while(getline (iffMM,line)){
		std::replace(line.begin(), line.end(), ';', ' ');
		istringstream iss(line);
		while(iss >> word){
			Metrics[count].push_back(atoi(word.c_str()));
		}
		count++;
	}
	iffMM.close();
	

// Load File MetricRequests
	ifstream iffMR(pathMR.c_str());
	
	if (!iffMR.is_open()){
		cout <<"Impossibile aprire il file"<<pathMR<<endl;
		cin.get();
		exit(1);
	}


	for(int i = 0; i < NAttivita; i++){
		getline (iffMR,line);
		std::replace(line.begin(), line.end(), ';', ' ');
		istringstream iss(line);
		while(iss >> word){
			if(word.compare("NS")!=0){
					MetricsRequest[i].push_back(atoi(word.c_str()));
			}
		}
	}
	
	iffMR.close();

	// Building the preference vector
	for (std::map<int,vector <int>>::iterator it=Metrics.begin(); it!=Metrics.end(); it++){
		GradimentoMetriche.push_back(rand()%10);
	}

}

int Instance::getNMetriche(){
	return NMetriche;
}

int Instance::getNAttivita(){
	return NAttivita;
}

int Instance::getNSiti(){
	return NSiti;
}

double Instance::getBudget(){
	return Budget;
}

double Instance::getPrecisione(int k, int j, int i){
	return Precisione[k][j][i];
}

double Instance::getDeltaZPrecisione(int k, int j, int i){
	return Delta1Precisione[k][j][i];
}

double Instance::getDeltaYPrecisione(int k, int j, int i){
	return Delta2Precisione[k][j][i];
}

double Instance::getPreferenze(int k){
	for (std::map<int,vector <int>>::iterator it=Metrics.begin(); it!=Metrics.end(); it++){
		for(int i = 0; i < it->second.size(); i++){
			if(it->second.at(i)==k){
				return GradimentoMetriche.at(it->first);
			}
		}
	}
	return 0.0;
}

double Instance::getMinAccuracy(){
	double min =1e05;

	for(int i=0; i < NAttivita; i++){
		for(int k=0; k < NMetriche; k++){
			for(int j=0; j < NSiti; j++){
				if(abs(Delta1Precisione[k][j][i])<min)
					min=abs(Precisione[k][j][i]);
				if(abs(Delta1Precisione[k][j][i])<min)
					min=abs(Delta1Precisione[k][j][i]);
				if(abs(Delta2Precisione[k][j][i])<min)
					min=abs(Delta1Precisione[k][j][i]);
			}
		}
	}

	return min;
}

vector<int> Instance::getAttivitaStesso(){
	return ElencoAttivitaStesso;
}

vector<int> Instance::getAttivitaNoStesso(){
	return ElencoAttivitaNoStesso;
}

map<int, vector<int>>  Instance::getSpecialRequest(){
	return SpecialRequest;
}

map<int, vector<int>>  Instance::getMetricsRequest(){
	return MetricsRequest;
}

map<int, vector<int>>  Instance::getMetrics(){
	
	//Creation of the partition induced by the metrics
	map<int, vector<int>>  NewMetrics=Metrics;

	int* frequenze= new int[NMetriche];
	for(int i = 0; i< NMetriche; i++){
		frequenze[i]=0;
	}

	for (std::map<int,vector <int>>::iterator it=Metrics.begin(); it!=Metrics.end(); it++){
		for(int i = 0; i < it->second.size(); i++){
				frequenze[it->second.at(i)]++;
		}
	}


	for(int i = 0; i< NMetriche; i++){
		// if frequenze is greater than one we have two metrics that share the same metric measurement
		if(frequenze[i]>1){
			vector<int> concorrenti;
			for (std::map<int,vector <int>>::iterator it=Metrics.begin(); it!=Metrics.end(); it++){
				for(int j = 0; j < it->second.size(); j++){
					if(it->second.at(j)==i)
						concorrenti.push_back(it->first);
				}
			}
			

			// in this case I will consider matrics with more gradimento 
			int concorrenteVincitore=0;
			int maxTemp=0;
			for(int j = 0; j < concorrenti.size(); j++){
				if(GradimentoMetriche[concorrenti.at(j)]>maxTemp){
					concorrenteVincitore=concorrenti.at(j);
					maxTemp = GradimentoMetriche[concorrenti.at(j)];
				}
			}
			

			// I have to delete the metric measurement in each vectors that is not the winner
			
			for(int j = 0; j < concorrenti.size(); j++){
				if(concorrenti.at(j)!= concorrenteVincitore){
					vector<int> temp;
					for(int k = 0; k < NewMetrics[concorrenti.at(j)].size(); k++){
						if(NewMetrics[concorrenti.at(j)].at(k)!=i)
							temp.push_back(NewMetrics[concorrenti.at(j)].at(k));
					}
					NewMetrics[concorrenti.at(j)]=temp;
				}
			}	
			

		}
	}


	Metrics=NewMetrics;

	return Metrics;
}

double Instance::getPrice(int j){
	return CostoFisso.at(j);
}

double Instance::getEstimationPrice(int k,int j){
	return CostoValutazione[k][j];
}

double Instance::getProbePrice(int k,int j){
	return CostoSonda[k][j];
}

void Instance::duplicaAttività(int a1, int a2, int nCopie){

	if(nCopie==0){
		return;
	}

	// Update NAttivita
	NAttivita += nCopie*2;

	//Update Precisione
	double*** NewPrecisione = new double**[NMetriche];
	for(int k = 0; k < NMetriche; k++){
			NewPrecisione[k] = new double*[NSiti];
			for(int j = 0; j < NSiti; j++){
				NewPrecisione[k][j] = new double[NAttivita];
			}
	} 
	for(int k = 0; k < NMetriche; k++){
		for(int j = 0; j < NSiti; j++){
			for(int i = 0; i < NAttivita-nCopie*2; i++){
				NewPrecisione[k][j][i] = Precisione[k][j][i];
			}
		}
	}

	for(int k = 0; k < NMetriche; k++){
		for(int j = 0; j < NSiti; j++){
			int index = NAttivita-2*nCopie;
			for(int c = 0; c <= 2*nCopie; c++){
				NewPrecisione[k][j][index + c] = Precisione[k][j][a1];
				NewPrecisione[k][j][index + c + 1] = Precisione[k][j][a2];
				c++;
			}
		}
	}
	Precisione = NewPrecisione;

	

	//Update Delta1Precisione
	double*** NewDelta1Precisione = new double**[NMetriche];
	for(int k = 0; k < NMetriche; k++){
			NewDelta1Precisione[k] = new double*[NSiti];
			for(int j = 0; j < NSiti; j++){
				NewDelta1Precisione[k][j] = new double[NAttivita];
			}
	} 
	for(int k = 0; k < NMetriche; k++){
		for(int j = 0; j < NSiti; j++){
			for(int i = 0; i < NAttivita-nCopie*2; i++){
				NewDelta1Precisione[k][j][i] = Delta1Precisione[k][j][i];
			}
		}
	}

	for(int k = 0; k < NMetriche; k++){
		for(int j = 0; j < NSiti; j++){
			int index = NAttivita-2*nCopie;
			for(int c = 0; c <= 2*nCopie; c++){
				NewDelta1Precisione[k][j][index + c] = Delta1Precisione[k][j][a1];
				NewDelta1Precisione[k][j][index + c + 1] = Delta1Precisione[k][j][a2];
				c++;
			}
		}
	}
	Delta1Precisione = NewDelta1Precisione;


	//Update Delta2Precisione
	double*** NewDelta2Precisione = new double**[NMetriche];
	for(int k = 0; k < NMetriche; k++){
			NewDelta2Precisione[k] = new double*[NSiti];
			for(int j = 0; j < NSiti; j++){
				NewDelta2Precisione[k][j] = new double[NAttivita];
			}
	} 
	for(int k = 0; k < NMetriche; k++){
		for(int j = 0; j < NSiti; j++){
			for(int i = 0; i < NAttivita-nCopie*2; i++){
				NewDelta2Precisione[k][j][i] = Delta2Precisione[k][j][i];
			}
		}
	}

	for(int k = 0; k < NMetriche; k++){
		for(int j = 0; j < NSiti; j++){
			int index = NAttivita-2*nCopie;
			for(int c = 0; c <= 2*nCopie; c++){
				NewDelta2Precisione[k][j][index + c] = Delta2Precisione[k][j][a1];
				NewDelta2Precisione[k][j][index + c + 1] = Delta2Precisione[k][j][a2];
				c++;
			}
		}
	}
	Delta2Precisione = NewDelta2Precisione;

	
	// Update ElencoAttivitàNoStesso
	vector<int> NewElencoAttivitaNoStesso = ElencoAttivitaNoStesso;

	for(int b = 0; b < ElencoAttivitaNoStesso.size(); b++){
		// se è il primo membro che ci assomiglia allora
		if(ElencoAttivitaNoStesso.at(b)==a1){
			int index = NAttivita-2*nCopie;
			for(int c = 0; c <= nCopie; c++){
				NewElencoAttivitaNoStesso.push_back(index + c);
				NewElencoAttivitaNoStesso.push_back(ElencoAttivitaNoStesso.at(b+1));
				c++;
			}
		}
		if(ElencoAttivitaNoStesso.at(b)==a2){
			int index = NAttivita-2*nCopie;
			for(int c = 0; c <= nCopie; c++){
				NewElencoAttivitaNoStesso.push_back(index + c+1);
				NewElencoAttivitaNoStesso.push_back(ElencoAttivitaNoStesso.at(b+1));
				c++;
			}
		}
		// se è il secondo membro che ci assomiglia allora
		if(ElencoAttivitaNoStesso.at(b+1)==a1){
			int index = NAttivita-2*nCopie;
			for(int c = 0; c <= nCopie; c++){
				NewElencoAttivitaNoStesso.push_back(ElencoAttivitaNoStesso.at(b));
				NewElencoAttivitaNoStesso.push_back(index + c);
				c++;
			}
		}
		if(ElencoAttivitaNoStesso.at(b+1)==a2){
			int index = NAttivita-2*nCopie;
			for(int c = 0; c <= nCopie; c++){
				NewElencoAttivitaNoStesso.push_back(ElencoAttivitaNoStesso.at(b));
				NewElencoAttivitaNoStesso.push_back(index + c+1);
				c++;
			}
		}
		b++;
	}
	ElencoAttivitaNoStesso = NewElencoAttivitaNoStesso;
	
	// Update ElencoAttivitaStesso
	vector<int> NewElencoAttivitaStesso = ElencoAttivitaStesso;

	for(int b = 0; b < ElencoAttivitaStesso.size(); b++){
		if((ElencoAttivitaStesso.at(b)==a1) && (ElencoAttivitaStesso.at(b+1)==a2)){
			int index = NAttivita-2*nCopie;
			for(int c = 0; c <= nCopie; c++){
				NewElencoAttivitaStesso.push_back(index + c);
				NewElencoAttivitaStesso.push_back(index + c + 1);
				c++;
			}
		}
		b++;
	}

	ElencoAttivitaStesso = NewElencoAttivitaStesso;

	// Update SpecialRequest
	map<int, vector<int>>  NewSpecialRequest;

	NewSpecialRequest=SpecialRequest;
	
	for (std::map<int,vector <int>>::iterator it=SpecialRequest.begin(); it!=SpecialRequest.end(); it++){
		if(it->first==a1){
			int index = NAttivita-2*nCopie;
			for(int c = 0; c <= nCopie; c++){
				NewSpecialRequest[index + c]=it->second;
				c++;
			}
		}
		if(it->first==a2){
			int index = NAttivita-2*nCopie;
			for(int c = 0; c <= nCopie; c++){
				NewSpecialRequest[index + c + 1]=it->second;
				c++;
			}
		}

	}

	SpecialRequest=NewSpecialRequest;
	


	// Update MetricRequest
	for(int c = 0; c < 2*nCopie; c++){
		for(int i = 0; i < MetricsRequest[a1].size(); i++){
			MetricsRequest[NAttivita-2*nCopie+c].push_back(MetricsRequest[a1].at(i));
		}
		for(int i = 0; i < MetricsRequest[a1].size(); i++){
			MetricsRequest[NAttivita-2*nCopie+c+1].push_back(MetricsRequest[a2].at(i));
		}
		c++;
	}

}

void Instance::setGradimentoMetriche(vector<double> Gradimento){
	this->GradimentoMetriche = Gradimento;
}

void Instance::Stampa(){
	
	cout<<"Constraints activity in the same site:"<<endl;
	for(int i=0; i<ElencoAttivitaStesso.size();i++){
		cout<<ElencoAttivitaStesso.at(i)<<" "<<ElencoAttivitaStesso.at(i+1)<<endl;
		i++;
	}

	cout<<endl;
	cout<<"Constraints activity NOT in the same site: "<<endl;
	for(int i=0; i<ElencoAttivitaNoStesso.size();i++){
		cout<<ElencoAttivitaNoStesso.at(i)<<" "<<ElencoAttivitaNoStesso.at(i+1)<<endl;
		i++;
	}
	
	cout<<endl;
	cout<<"Special Constraints:"<<endl;
	for (std::map<int,vector <int>>::iterator it=SpecialRequest.begin(); it!=SpecialRequest.end(); it++){
		cout << it->first<<"	";
		for(int i = 0; i < it->second.size(); i++){
			cout<<it->second.at(i)<<" ";
		}	
		cout<<endl;
	}

	
	cout<<endl;
	cout<<"Fixed Costs:"<<endl;
	for(int i=0; i < CostoFisso.size();i++){
		cout<<CostoFisso.at(i)<<endl;
	}

	cout<<endl;
	cout<<"Probe Costs:"<<endl;
	for(int k=0; k < NMetriche; k++){
		for(int j=0; j < NSiti; j++){
			cout<< CostoSonda[k][j]<<" ";
		}
		cout<<endl;
	}
	
	cout<<endl;
	cout<<"Evaluation Costs:"<<endl;
	for(int k=0; k < NMetriche; k++){
		for(int j=0; j < NSiti; j++){
			cout<< CostoValutazione[k][j]<<" ";
		}
		cout<<endl;
	}

	
	cout<<endl;
	cout<<"Standard Precision:"<<endl;
	for(int i=0; i < NAttivita; i++){
		cout<<"Activity "<<i<<": "<<endl;
		for(int k=0; k < NMetriche; k++){
			for(int j=0; j < NSiti; j++){
				cout<< Precisione[k][j][i]<<" ";
			}
			cout<<endl;
		}
	}
	
	cout<<endl;
	cout<<"Variation if evaluation:"<<endl;
	for(int i=0; i < NAttivita; i++){
		cout<<"Activity "<<i<<": "<<endl;
		for(int k=0; k < NMetriche; k++){
			for(int j=0; j < NSiti; j++){
				cout<< Delta1Precisione[k][j][i]<<" ";
			}
			cout<<endl;
		}
	}
	
	cout<<endl;
	cout<<"Variation if probes:"<<endl;
	for(int i=0; i < NAttivita; i++){
		cout<<"Activity "<<i<<": "<<endl;
		for(int k=0; k < NMetriche; k++){
			for(int j=0; j < NSiti; j++){
				cout<< Delta2Precisione[k][j][i]<<" ";
			}
			cout<<endl;
		}
	}

	cout<<endl;
	cout<<"Metrics :"<<endl;
	for (std::map<int,vector <int>>::iterator it=Metrics.begin(); it!=Metrics.end(); it++){
		for(int i = 0; i < it->second.size(); i++){
			cout<<it->second.at(i)<<" ";
		}	
		cout<<endl;
	}

	cout<<endl;
	cout<<"RequestedMetrics :"<<endl;
	for (std::map<int,vector <int>>::iterator it=MetricsRequest.begin(); it!=MetricsRequest.end(); it++){
		cout<<it->first<<"  ";
		for(int i = 0; i < it->second.size(); i++){
			cout<<it->second.at(i)<<" ";
		}	
		cout<<endl;
	}
	
	cout<<endl;
	cout<<"Budget: "<<Budget <<endl;
	
	cout<<endl;
	cout<<"NActivity: "<<NAttivita<<" NSites: "<<NSiti<<" NMetricsMeasurements: "<<NMetriche <<endl;
}


void Instance::consideraAttivitàFrom(int to){
	NMetriche=to+1;

	map<int,vector <int>> newMetrics;
	for (std::map<int,vector <int>>::iterator it=Metrics.begin(); it!=Metrics.end(); it++){
		for(int i = 0; i < it->second.size(); i++){
			if(it->second.at(i)<=to){
				newMetrics[it->first].push_back(it->second.at(i));
			}
		}	
	}
	Metrics=newMetrics;
	

	map<int,vector <int>> newMetricsRequest;
	for(std::map<int,vector <int>>::iterator it=MetricsRequest.begin(); it!=MetricsRequest.end(); it++){
		for(int i = 0; i < it->second.size(); i++){
			if(it->second.at(i)<=to){
				newMetricsRequest[it->first].push_back(it->second.at(i));
			}
		}	
	}

	MetricsRequest=newMetricsRequest;

}
