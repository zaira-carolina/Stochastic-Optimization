#include <iostream>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>
#include <map>
#include <sys/time.h>

// tomo la bolsa que tenga menos capacidad
// y la dejo estatica para la siguiente evaluacion

using namespace std;

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


int busEsto(vector<int> weight, int c){
	
	int contenedores = 0;
	vector<int> bins;
    vector<int>::iterator lower; 
    multimap<int, int> mapa; 
    multimap<int, int>::iterator itr;
    
    while(weight.size() > 0){   
        int i = 0, index;
        random_shuffle(weight.begin(), weight.end());


        mapa.clear();
        bins.push_back(c);

        while (weight.size() > 0){

            lower = lower_bound(bins.begin(), bins.end(), weight[i]);
            index = lower-bins.begin();
            
            if (index <= (bins.size()-1)){
                bins[index] -= weight[i]; 
                // hacer un multimapa para que tenga todos los pesos que estan en el contenedor
                mapa.insert(make_pair(index, weight[i])); 

                weight[i] = weight[weight.size()-1];
                weight.pop_back(); 
                i++; 
            }
            else{
                bins.push_back(c); 
            }
            
        }
        
        contenedores += 1; 
		// encontrar un espacio vacio
        int indice = 0;
        for (int l=0; l<bins.size(); l++){
            if (bins[l] == 0){
                indice = l;
            }
        }
        
        // llenar weight con los elementos restantes 
        for (auto itr = mapa.begin(); itr!= mapa.end(); itr++){
            if (itr->first != indice){
                weight.push_back(itr->second);
                //cout << itr->second << endl; 
            }
        }
        
        
        
        bins.clear(); 
        mapa.clear(); 
        
    }
    return contenedores;
}


int main(){
	
	struct timeval start, end;
	
	int size, c;
	char instancia1[30] = "Falkenauer_u500_07.txt";
	int *w = readFile(instancia1, &size, &c);
	
	char name_end[30] = "Falkenauer_u500_07_SOL_b.txt";
	char time_end[30] = "Falkenauer_u500_07_TIME_b.txt";
	
	vector<int> weight;
	for (int i=0; i<size; i++){
		weight.push_back(w[i]);
	}
	
	
	int res, iter = 50;
	double solutions[iter] = { };
	double tiempo[iter] = { };
	for (int i=0; i<iter; i++){
		
		gettimeofday(&start, NULL);
		
		srand(i);
		res = busEsto(weight, c); 
		solutions[i] = (double)res;
		
		gettimeofday(&end, NULL);
		tiempo[i] = (end.tv_sec*1.0e6+end.tv_usec)-(start.tv_sec*1.0e6+start.tv_usec);
		
	}

	write_vector(solutions, iter,  name_end);
	write_vector(tiempo, iter, time_end);
		

}

