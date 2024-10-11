// Evaluador 
#include <iostream>
#include <vector>
#include <algorithm> // Librería para utilizar random_shuffle
#include <time.h>
#include <stdio.h> // Librería para leer el archivo txt 
#include <string>
#include <cstdlib> // rand
#include <omp.h>

/* Info sobre los dados: 
 * Para este conjunto de instancias, se tiene el mismo valor máximo para cada contenedor.
 * Los valores son enteros desde 20 hasta 100. Hay 120 valores por instancia. 
 * Según los artículos, la cantidad óptima de contenedores está entre [46, 52]. */

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

int myrandom(int i){return rand()%i; }

int randomSol(int *array, int size, int contenedor, int semilla){
	// Reordenar 
	srand(semilla); 
	random_shuffle(&array[0], &array[size-1]); 
	int suma = 0, contador = 0, elemento = 0;
	
	while (elemento < size){
		// Intenta meter elementos al contenedor. 
		if (suma <= contenedor){
			suma+= array[elemento];
			elemento++; 
		}
		if (suma > contenedor){
			// Se reinicia la suma cuando supera el espacio del contenedor. 
			suma = 0; 
			contador++; 
			elemento--; 
		}
	} 
	return contador; 
}

void printArray(int *array, int size){
	for (int i=0; i<size; i++){
		cout << array[i] << endl; 
	}
}

double evaluador(int sol){
	int solucionAceptada = 52.0; 
	double errorRelativo = 0; 
	errorRelativo = ((double)sol-solucionAceptada)/(solucionAceptada);
	return errorRelativo; 
}

void write_vector(int *vec, int N, char *nombre){
	FILE *file; 
	if ((file = fopen(nombre, "w")) == NULL){
		cout << "El archivo no pudo abrirse. " << endl;  
	}
	else{
		fprintf(file, "%d\n", N); 
		for (int i = 0; i<N; i++){
			fprintf(file, "%d\n", vec[i]);
		}
		fclose(file);
	}
}


int main(){
	//srand(time(0)); 
	
	int size, maxSuma;
	char instancia1[30] = "Falkenauer_u120_01.txt";
	int *inst1 = readFile(instancia1, &size, &maxSuma); 
	int N = 100000;
	int res[N] = { }; 
	int sol[100] = { };
	char name_end[30] = "res_instancia1_paralelo.txt";

	#pragma omp parallel for 
	for (int j = 0; j<100; j++){
		for (int i=0; i<N; i++){
			int c = randomSol(inst1, size, maxSuma, j); 
			res[i] = c;  
		}

		int min = *min_element(res, res+N); 
		cout << min << endl; 
		sol[j] = min; 
	}
	write_vector(sol, 100,  name_end); 


	return 0; 
}




