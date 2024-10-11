#include <bits/stdc++.h>
#include <vector>
#include <iostream>
#include <sys/time.h>
#include <stdio.h>
#include <algorithm>

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

int BFD(vector<int> weight, int c){
	vector<int> bins;
	vector<int>::iterator lower; 
	
	bins.push_back(c);
	random_shuffle(weight.begin(), weight.end());
	
	int i = 0, index;
	
	while (weight.size() > 0){

		// buscar un contenedor con espacio
		lower = lower_bound(bins.begin(), bins.end(), weight[i]);
		// indice del contenedor con espacio
		index = lower-bins.begin();

        if (index <= (bins.size()-1)){
			// ingresar el elemento al contenedor
			bins[index] -= weight[i]; 
			// mover el elemento para borrarse
            weight[i] = weight[weight.size()-1];
            weight.pop_back(); 
            //moverme al siguiente elemento
            i++; 
		}
        else{
			// en otro caso, anadir un nuevo contenedor
            bins.push_back(c); 
        }
	 }   
	return (bins.size());
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


int main(){
	
	struct timeval start, end;
	gettimeofday(&start, NULL);

	int size, c;
	char instancia1[30] = "Falkenauer_u500_07.txt";
	int *w = readFile(instancia1, &size, &c);
	
	char name_end[30] = "Falkenauer_u500_07_SOL_a.txt";
	char time_end[30] = "Falkenauer_u500_07_TIME_a.txt";
	
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
		res = BFD(weight, c); 
		solutions[i] = (double)res;
		
		gettimeofday(&end, NULL);
		tiempo[i] = (end.tv_sec*1.0e6+end.tv_usec)-(start.tv_sec*1.0e6+start.tv_usec);
		
	}

	write_vector(solutions, iter,  name_end);
	write_vector(tiempo, iter, time_end);
	
	

	return 0;
}
