#include "gurobi_c++.h"
#include "instance.h"

#include <sstream>
#include <string>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <cmath>
#include <deque>
#include <iomanip>

using namespace std;

string itos(int i) {stringstream s; s << i; return s.str(); }


int main(int   argc,char *argv[]){

	/*
	Instance* in=new Instance("../Cluster/Input/Together.csv",
		"../Cluster/Input/NoTogether.csv",
		"../Cluster/Input/SpecialRequests.csv",
		"../Cluster/Input/EstimationCosts.csv",
		"../Cluster/Input/FixedCosts.csv",
		"../Cluster/Input/ProbeCosts.csv",
		"../Cluster/Input/Precision.csv",
		"../Cluster/Input/Delta1Precisions.csv",
		"../Cluster/Input/Delta2Precisions.csv",
		"../Cluster/Input/BudegetEnumeroAttivita.csv",
		"../Cluster/Input/MetricMeasurements.csv",
		"../Cluster/Input/MeasuresRequest.csv");
		
	*/
	
	Instance* in=new Instance("C:/Users/edofa/Dropbox/BackUp Lavori/ArticoloPlebani/Cluster/Input/Together.csv",
		"C:/Users/edofa/Dropbox/BackUp Lavori/ArticoloPlebani/Cluster/Input/NoTogether.csv",
		"C:/Users/edofa/Dropbox/BackUp Lavori/ArticoloPlebani/Cluster/Input/SpecialRequests.csv",
		"C:/Users/edofa/Dropbox/BackUp Lavori/ArticoloPlebani/Cluster/Input/EstimationCosts.csv",
		"C:/Users/edofa/Dropbox/BackUp Lavori/ArticoloPlebani/Cluster/Input/FixedCosts.csv",
		"C:/Users/edofa/Dropbox/BackUp Lavori/ArticoloPlebani/Cluster/Input/ProbeCosts.csv",
		"C:/Users/edofa/Dropbox/BackUp Lavori/ArticoloPlebani/Cluster/Input/Precision.csv",
		"C:/Users/edofa/Dropbox/BackUp Lavori/ArticoloPlebani/Cluster/Input/Delta1Precisions.csv",
		"C:/Users/edofa/Dropbox/BackUp Lavori/ArticoloPlebani/Cluster/Input/Delta2Precisions.csv",
		"C:/Users/edofa/Dropbox/BackUp Lavori/ArticoloPlebani/Cluster/Input/BudegetEnumeroAttivita.csv",
		"C:/Users/edofa/Dropbox/BackUp Lavori/ArticoloPlebani/Cluster/Input/MetricMeasurements.csv",
		"C:/Users/edofa/Dropbox/BackUp Lavori/ArticoloPlebani/Cluster/Input/MeasuresRequest.csv");

	in -> duplicaAttività(2,3,600);
//for(int index = 0; index <= 16; index ++){
	in->consideraAttivitàFrom(0);
	//in->Stampa();

	
	int NMetriche	=	in->getNMetriche();
	int NSiti	=	in->getNSiti();
	int NAttivita	=	in->getNAttivita();



	double* Preferenze = new double[NMetriche];
	
//for(int index = 0; index < 40; index ++){
	
	vector<double> Gradimento;
	Gradimento.push_back(1); // 0
	Gradimento.push_back(1); // 1
	Gradimento.push_back(1); // 2
	Gradimento.push_back(1); // 3
	Gradimento.push_back(1); // 4
	Gradimento.push_back(1); // 5
	Gradimento.push_back(1); // 6
	Gradimento.push_back(1); // 7

	in->setGradimentoMetriche(Gradimento);
	


	map<int, vector<int>>  Metrics = in->getMetrics(); // se non chiamo get Metrics non ho una partizione per Metrics
	map<int, vector<int>> MetricsRequest = in->getMetricsRequest();
	
	for(int k = 0; k < NMetriche; k++){
		Preferenze[k] = in->getPreferenze(k);
	}

    
	try{
			clock_t tStart = clock();
			
			GRBEnv env = GRBEnv();
			GRBModel model = GRBModel(env);
			
			//w{j}
			GRBVar *w = new GRBVar[NSiti];
			for (int j = 0; j < NSiti; j++){
				string name = "w" + itos(j);
				w[j]= model.addVar(0.0, GRB_BINARY, 0.0, GRB_BINARY, name);
			}

			// x{ij}
			GRBVar **x = new GRBVar*[NAttivita];
			for(int i = 0; i < NAttivita; i++){
				x[i] = new GRBVar[NSiti];
			}
			for (int i = 0; i < NAttivita; i++){
				for (int j = 0; j < NSiti; j++){
						string name = "x" + itos(i) + "_" + itos(j);
						x[i][j]= model.addVar(0.0, GRB_BINARY, 0.0, GRB_BINARY, name);
				}
			}
			
			// z_{kj}
			GRBVar **z = new GRBVar*[NMetriche];
			for(int i = 0; i < NMetriche; i++){
				z[i] = new GRBVar[NSiti];
			}
			for (int i = 0; i < NMetriche; i++){
				for (int j = 0; j < NSiti; j++){
						string name = "z_" + itos(i) + "_" + itos(j);
						z[i][j] = model.addVar(0.0, GRB_BINARY, 0.0, GRB_BINARY, name);
				}
			}

			// y_{kj}
			GRBVar **y = new GRBVar*[NMetriche];
			for(int i = 0; i < NMetriche; i++){
				y[i] = new GRBVar[NSiti];
			}

			for (int i = 0; i < NMetriche; i++){
				for (int j = 0; j < NSiti; j++){
						string name = "y_" + itos(i) + "_" + itos(j);
						y[i][j] = model.addVar(0.0, GRB_BINARY, 0.0, GRB_BINARY, name);
				}
			}

			// l: budget limit
			GRBVar l = model.addVar(-1e20 ,1e20,-1.0,GRB_CONTINUOUS,"l");
			
			// u_{ikj}
			GRBVar ***u = new GRBVar**[NAttivita];
			
			for(int i = 0; i < NAttivita; i++){
				u[i] = new GRBVar*[NSiti];
				for (int j = 0; j < NSiti; j++)
					u[i][j] = new GRBVar[NMetriche];
			}

			for (int i = 0; i < NAttivita; i++){
				for (int j = 0; j < NSiti; j++){
					for (int k = 0; k < NMetriche; k++){
						string name = "u_" + itos(i) + "_" + itos(j)+"_" + itos(k);
						u[i][j][k] = model.addVar(0.0, GRB_BINARY, 0.0, GRB_BINARY, name);
					}
				}
			}

			// Integrate variables into model
			model.update();
			GRBLinExpr obj;
			GRBLinExpr expr=0;
			
			
			for(int k = 0; k < NMetriche; k++){
				expr=0;
				for (int j = 0; j < NSiti; j++){
					double deltaZprecisione = 0.0;
					double deltaYprecisione = 0.0;

					for (int i = 0; i < NAttivita; i++){
						expr += in->getPrecisione(k,j,i)*x[i][j];
						deltaZprecisione += in->getDeltaZPrecisione(k,j,i);
						deltaYprecisione += in->getDeltaYPrecisione(k,j,i);
					}
					expr += deltaZprecisione*z[k][j] + deltaYprecisione*y[k][j];
				}
				obj+= Preferenze[k]*expr;
			}

			obj-=l;
			
			model.setObjective(obj,GRB_MAXIMIZE); 

			
			// Add constraints
		
			//  
			for (int j = 0; j < NSiti; j++){
				for(int i = 0; i < NAttivita; i++){
					model.addConstr(w[j], GRB_GREATER_EQUAL, x[i][j]);	
				}
			}
			

			//  
			for(int i = 0; i < NAttivita; i++){
				expr=0;
				for (int j = 0; j < NSiti; j++){
					expr += x[i][j];	
				}
				model.addConstr(expr, GRB_EQUAL, 1);
			}

			
			//  
			for (int k = 0; k < NMetriche; k++) {
				for (int j = 0; j < NSiti; j++){
					expr=0;
					for (int i = 0; i < NAttivita; i++){
						expr += x[i][j];
					}
					model.addConstr(y[k][j], GRB_LESS_EQUAL, expr);
				}
			}

			
			// 
			for (int k = 0; k < NMetriche; k++) {
				for (int j = 0; j < NSiti; j++){
					for (int i = 0; i < NAttivita; i++){
						model.addConstr(z[k][j], GRB_LESS_EQUAL, 1-in->getPrecisione(k,j,i));
					}
				}
			}

			
			//  
			for (int k = 0; k < NMetriche; k++) {
				for (int j = 0; j < NSiti; j++){
					expr=0;
					for (int i = 0; i < NAttivita; i++){
						expr += x[i][j];
					}
					model.addConstr(z[k][j], GRB_LESS_EQUAL, expr);
				}
			}

			
			// 
			vector<int> ElencoAttivitaStesso = in->getAttivitaStesso();
			for(int b = 0; b < ElencoAttivitaStesso.size(); b++){
				for (int j = 0; j < NSiti; j++){
					model.addConstr(x[ElencoAttivitaStesso.at(b)][j], GRB_EQUAL, x[ElencoAttivitaStesso.at(b+1)][j]);
				}
				b++;
			}

			
			
			//  
			vector<int> ElencoAttivitaNoStesso = in->getAttivitaNoStesso();
			for(int b = 0; b < ElencoAttivitaNoStesso.size(); b++){
				for (int j = 0; j < NSiti; j++){
					model.addConstr(x[ElencoAttivitaNoStesso.at(b)][j]+x[ElencoAttivitaNoStesso.at(b+1)][j], GRB_LESS_EQUAL, 1);
				}
				b++;
			}
			
			
			//  
			map<int, vector<int>>  SpecialRequest=in->getSpecialRequest();
			for (std::map<int,vector <int>>::iterator it=SpecialRequest.begin(); it!=SpecialRequest.end(); it++){
				expr=0;
				for(int i = 0; i < it->second.size(); i++){
					expr+=x[it->first][it->second.at(i)];
				}	
				model.addConstr(expr, GRB_EQUAL, 1);
			}
			
			

			//  
			expr=0;
			for (int j = 0; j < NSiti; j++){
				expr += w[j]*in->getPrice(j);
			}

			for(int k = 0; k < NMetriche; k++){
				for (int j = 0; j < NSiti; j++){
					expr+=in->getEstimationPrice(k,j)*z[k][j];
				}
			}

			for(int k = 0; k < NMetriche; k++){
				for (int j = 0; j < NSiti; j++){
					expr+=in->getProbePrice(k,j)*y[k][j];
				}
			}
			
			model.addConstr(expr, GRB_LESS_EQUAL, in->getBudget()+l);



			
			for (std::map<int,vector <int>>::iterator it=MetricsRequest.begin(); it!=MetricsRequest.end(); it++){
				for(int count = 0; count < it->second.size(); count++){
					int k = it->second.at(count);
					int i = it->first;
	
					for(int j = 0; j < NSiti; j++){
							expr =  in->getPrecisione(k,j,i)*x[i][j] + in->getDeltaZPrecisione(k,j,i)*z[k][j] + in->getDeltaYPrecisione(k,j,i)*y[k][j] - in->getMinAccuracy() ;
							model.addConstr(expr, GRB_LESS_EQUAL, u[i][j][k]);
							model.addConstr(-expr, GRB_LESS_EQUAL, 1-u[i][j][k]);
					}
					
				}	
			}
			
			for (std::map<int,vector <int>>::iterator it=MetricsRequest.begin(); it!=MetricsRequest.end(); it++){
				for(int count = 0; count < it->second.size(); count++){
					expr = 0;
					int k = it->second.at(count);
					int i = it->first;	
					for(int j = 0; j < NSiti; j++){
							expr += u[i][j][k];
					}
					model.addConstr(expr, GRB_GREATER_EQUAL, 1);
				}	
			}
			

			// Optimize model
			model.getEnv().set(GRB_IntParam_OutputFlag, 0);

			
			model.optimize();	
			
			cout<<(double)(clock() - tStart)/CLOCKS_PER_SEC << endl;



			ofstream fileO("C:/Users/edofa/Dropbox/BackUp Lavori/ArticoloPlebani/Cluster/soluzione.txt");

			fileO << "OBJ: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
			
			
			for(int j = 0; j<NSiti; j++){
				int flag=0;
				for(int i=0;i<NAttivita;i++){
					if(x[i][j].get(GRB_DoubleAttr_X) > 0.5){
						flag++;
					}
				}
				
				if(flag>0){
					fileO << "SITE " << j << endl;
					fileO << "\t Activity:" << endl;
					for(int i = 0; i < NAttivita; i++){
						if(x[i][j].get(GRB_DoubleAttr_X) > 0.5){
							fileO<< "\t" << i << "||  ";
							double s=0.0;
							for(int k = 0; k < NMetriche; k++){
								s += in->getPrecisione(k,j,i);
							}
							fileO<<" "<<s<<endl;
						}
					}
					fileO<<endl;
					fileO << "\t Evaluated Metrics:" << endl;
					int NStimate=0;
					for(int k = 0; k < NMetriche; k++){
						if(z[k][j].get(GRB_DoubleAttr_X) > 0.5){
							fileO<< "\t" << k << "   ";
							NStimate++;
						}
					}
					if(NStimate==0){
						fileO<<"\t ----";
					}

					fileO<<endl;

					fileO << "\t Probes:" << endl;
					int NSonde=0;
					for(int k = 0; k < NMetriche; k++){
						if(y[k][j].get(GRB_DoubleAttr_X) > 0.5){
							fileO<< "\t" << k << "   ";	
							NSonde++;
						}
					}

					if(NSonde==0){
						fileO<<"\t ----";
					}

					fileO<<endl;
					fileO<<endl;

				}
			}

			fileO <<"SolutionCosts: "<<endl;
			fileO <<"Std: ";
			double costo=0;
			for (int j = 0; j < NSiti; j++){
				if(w[j].get(GRB_DoubleAttr_X) > 0.5){
					fileO<<j<<"-" <<in->getPrice(j) <<" ";
					costo += in->getPrice(j);
				}
			}
			fileO <<endl <<"Estimations: ";
			for(int k = 0; k < NMetriche; k++){
				for (int j = 0; j < NSiti; j++){
					if(z[k][j].get(GRB_DoubleAttr_X) > 0.5){
						fileO<< in->getEstimationPrice(k,j) <<" ";
						costo += in->getEstimationPrice(k,j);
					}
				}
			}
			fileO <<endl <<"probes: ";
			for(int k = 0; k < NMetriche; k++){
				for (int j = 0; j < NSiti; j++){
					if(y[k][j].get(GRB_DoubleAttr_X) > 0.5){
						fileO<< in->getProbePrice(k,j) <<" ";
						expr+=in->getProbePrice(k,j);
					}
				}
			}

			fileO<<endl <<"Total Cost: "<<costo <<" l=: "<<l.get(GRB_DoubleAttr_X)<<endl;
			
			fileO<<endl<<"("<<l.get(GRB_DoubleAttr_X)<<","<< model.get(GRB_DoubleAttr_ObjVal)-l.get(GRB_DoubleAttr_X)<<")" <<endl;

			fileO.close();

		}catch(GRBException e) {
			cout << "Error code = " << e.getErrorCode() << endl;
			cout << e.getMessage() << endl;
		}catch (...) {
			cout << "Error during optimization" << endl;
		}
//}		
	cin.get();
	return 0;
}