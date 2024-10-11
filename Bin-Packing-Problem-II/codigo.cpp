#include <bits/stdc++.h>
#include <iostream>
#include <time.h> 
#include <cmath>
using namespace std;

vector<int> weight; 
vector<double> residual; 
vector<double> Dweight; // distortion weight 
vector<vector<double>> bins; 
int c; 


int *readFile(char *nombre, int *size, int *maxContenedor){
	FILE *file; 
	int *array, s, m; 
	
	if ((file = fopen(nombre, "r")) == NULL){
		cout << "El archivo no pudo abrirse. " << endl;  
	}
	else{
		fscanf(file, "%d%d", &s, &m); 
		array = new int[s]; 
		for (int i = 0; i<s; i++){
			fscanf(file, "%d", &array[i]); 
		}
		fclose(file);
	}
	*size = s;
	*maxContenedor = m; 
	return array;
}

void write_vector(double *vec, int N, char *nombre){
	FILE *file; 
	if ((file = fopen(nombre, "w")) == NULL){
		cout << "El archivo no pudo abrirse. " << endl;  
	}
	else{
		fprintf(file, "%d\n", N); 
		for (int i = 0; i<N; i++){
			fprintf(file, "%f\n", vec[i]);
		}
		fclose(file);
	}
}


int firstFit(int size){
	int res = 1; 
	residual.push_back(c-weight[0]);
	bins[0].push_back(weight[0]); 

	int j; 
	for (int i=1; i<size; i++){
		for (j=0; j<res; j++){
			if (residual[j] > 0 && residual[j] >= weight[i]){
				residual[j] -= weight[i];
				bins[j].push_back(weight[i]); 
				break; 
			}
		}
		
		if (j == res){
			residual.push_back(c-weight[i]);
			bins[res].push_back(weight[i]);
			res++;	
		}
	}
	return res;
}


void distortion(double K, double T){
	int n = residual.size(); 
	for (int i=0; i<n; i++){
		double r = (double)residual[i]/(double)c; 
		double change = pow((1+K*r), T);
		Dweight.push_back(change); 
	}
}

void applyDistortion(){
	for (int j=0; j<residual.size(); j++){
		//cout << "j: " << j << " Dweight[j]: " << Dweight[j] << endl; 
		int n = bins[j].size(); 
		//cout << "n: " << n << endl;
		double suma = 0;
		for (int i=0; i<n; i++){
			bins[j][i] *= Dweight[j]; 
			suma += bins[j][i]; 
			//cout << bins[j][i] << endl; 
		}
		residual[j] = c-suma; 
	}
}


double deltaF0(int i, int alpha, int beta){
	double lalpha, lbeta, talphai;
	
	lalpha = c-residual[alpha]; 
	lbeta = c-residual[beta];
	talphai = bins[alpha][i];
	
	double delta; 
	delta = (lalpha-talphai)*(lalpha-talphai)+(lbeta+talphai)*(lbeta+talphai)-lalpha*lalpha-lbeta*lbeta;
	return delta;
}

double deltaF1(int i, int j, int alpha, int beta){
	double lalpha, lbeta, talphai, tbetaj; 
	
	lalpha = c-residual[alpha];
	lbeta = c-residual[beta]; 
	talphai = bins[alpha][i];
	tbetaj = bins[beta][j];
	
	double delta; 
	delta = (lalpha-talphai+tbetaj)*(lalpha-talphai+tbetaj)+(lbeta-tbetaj+talphai)*(lbeta-tbetaj+talphai) +(lbeta-tbetaj+talphai)*(lbeta-tbetaj+talphai)-lalpha*lalpha-lbeta*lbeta; 
	return delta;
}

double lowerBound(int size){
	int bound;
	int suma = 0;
	for (int i = 0; i<size; i++){
		suma += weight[i];
	}
	double res = (1/(double)c)*(suma);
	return res;
}	

int main(){
	srand(time(NULL)); 
	
	int size;
	char instancia1[30] = "Falkenauer_u500_07.txt";
	int *w = readFile(instancia1, &size, &c);
	
	for (int i=0; i<size; i++){
		weight.push_back(w[i]);
	}
	
	int lb = lowerBound(size); // lower bound
	//cout << "lb: " << lb << endl; 
	bins.resize(lb+50); 
	
	int binsFirst = firstFit(size); 
	cout << "First fit: " << binsFirst << endl; 
	
	
	// Lo deje aqui porque no logre quitar menos objetos :(
	
	double K = 0.001, T = 1, Tred = 0.98; 
	for (int nloop = 0; nloop < 1000; nloop++){
		distortion(K, T); 
		applyDistortion();
		
		int alpha = 0, beta = 1; 
		//cout << "residual[alpha] = " << residual[alpha] << " residual[beta] = " << residual[beta] << endl; 
		while (beta < residual.size() ){
			// swap (1, 0) 
			for (int i = 0; i < bins[alpha].size(); i++){
				if (residual[beta] > 0){
					//cout << "here: " << endl;  
					double delta01 = deltaF0(i, alpha, beta);
					//cout << "alpha: " << alpha << " beta: " << beta << endl;  
					//cout << "delta01: " << delta01 << endl; 
					
					if (residual[beta] - bins[alpha][i] > 0 && delta01 >= 0 && residual.size() > lb){
						cout << "alpha: " << alpha << " beta: " << beta << endl; 
						cout << "bins[alpha].size()-begin: " << bins[alpha].size() << endl; 
						residual[beta] -= bins[alpha][i]; 
						residual[alpha] += bins[alpha][i];
						
						bins[beta].push_back(bins[alpha][i]);
						bins[alpha][i] = bins[alpha][bins[alpha].size()-1]; 
						bins[alpha].pop_back();
					
						cout << "bins[alpha].size()-end: " << bins[alpha].size() << endl; // << " residual[alpha]: " << residual[alpha] << endl; 
					}
					
					
					if (bins[alpha].size() == 0){
						cout << "delete alpha: " << alpha << endl; 
						residual[alpha] = residual[residual.size()]; 
						residual.pop_back(); 
						
						bins[alpha] = bins[residual.size()]; 
						bins.pop_back(); 
					}
					
				}
			}
			
			int jj = rand() % bins[beta].size(); 	
			int ii = rand() % bins[alpha].size(); 
			//cout << "ii: " << ii << " jj: " << jj << endl; 
				
			double u = residual[alpha]+ bins[alpha][ii];
			double v = residual[beta] + bins[beta][jj];
			double delta11 = deltaF1(ii, jj, alpha, beta); 
			
			
			if (u - bins[beta][jj] >= 0 && v - bins[alpha][ii] >= 0 && delta11 >= 0){
				//cout << "swap: " << endl; 
				residual[alpha] = u - bins[beta][jj];
				residual[beta] = v - bins[alpha][ii];
				
				swap(bins[beta][jj], bins[alpha][ii]); 
				
			}
			alpha++;
			beta++;
		}
		T *= Tred; 
	}
	
	
	
	
	
}
