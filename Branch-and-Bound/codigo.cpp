#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <time.h>

using namespace std;

int c = 100;
int n = 10; 
int arr[] = {49, 41, 34, 33, 29, 26, 26, 22, 20, 19}; 
vector<int> v(arr, arr+n); 


int alpha = 19;
vector<vector<int>> T; 
vector<int> LB; 
vector<int> depth;


int L(vector<int> arr, int alpha){
	vector<int>::iterator j1, j2, j2aux, j3, j3aux; 
	int diff = c-alpha;
	int mitad = c/2;
	int sumaj2 = 0, sumaj3 = 0;
	
	
	j1 = upper_bound(arr.begin(), arr.end(), diff, greater<int>());
	double J1 = j1-arr.begin(); 
	
	j2 = upper_bound(arr.begin(), arr.end(), mitad, greater<int>());
	j2aux = lower_bound(arr.begin(), arr.end(), diff+1, greater<int>());
	double J2 = fabs(j2aux-j2);
	
	if (J2 > 0){
		for (int i=(j2aux-arr.begin()); i<(j2-arr.begin()); i++){
			sumaj2 += arr[i];
		}
	}
	
	j3 = upper_bound(arr.begin(), arr.end(), alpha, greater<int>());
	j3aux = lower_bound(arr.begin(), arr.end(), mitad+1, greater<int>());
	double J3 = fabs(j3aux-j3);
	
	if (J3 > 0){
		for (int i=(j3aux-arr.begin()); i<(j3-arr.begin()); i++){
			sumaj3 += arr[i];
		}
	}
	
	int aux = ceil((sumaj3-(fabs(J2)*c-sumaj2))/c);
	int L = fabs(J1) + fabs(J2) + max(0, aux);
	return L; 
}

int lowerBound(vector<int> array, int alpha, int index){
	for (int i=index; i<n; i++){
		array.push_back(arr[i]);
	}
	 
	int l = L(array, alpha);
	return l; 
}

int same(vector<int> array, int index){
	for (int i=index; i<n; i++){
		array.push_back(arr[i]);
	}
	
	if (array == v){ return 1;}
	else return 0;
}

void tree(int node, int index, int aux, int LL){
	// calcular la profundidad de cada nodo
	depth.push_back(index);
	//cout << "node: " << node << " index: " << index << " aux: " << aux << endl;
	
	int len = T[node].size();
	for (int i=0; i<len; i++){
		int suma = T[node][i] + arr[index];
		if (suma <= c){
			aux++;
			T[aux].push_back(suma);
			// copiar los elementos restantes
			for (int elem = 0; elem < len; elem++){
				if (elem != i) { T[aux].push_back(T[node][elem]); }
			}
		}
	}
	// calcular L del primer aux
	int l = lowerBound(T[node], alpha, index);
	int s = same(T[node], index);
	if (l <= LL && s == 0){ LB.push_back(node);}
	
	
	// separar 
	aux++;
	for (int i=0; i<len; i++){
		T[aux].push_back(T[node][i]);
	}
	// Calcular L, si la supera, eliminar ese nodo y bajar los aux
	// anadir el indice 
	T[aux].push_back(arr[index]);
	if (T[aux].size()-1 == index){
		index++;
	}
	
	// funcion recursiva
	node++;
	if (index < n && index < 5){ tree(node, index, aux, LL); }
	else return; 
}

void firstFit(vector<int> arr, int node, int index){

	if (arr.size() > index){
		int randomSeed = depth[index] + rand() % (arr.size()-depth[index]);
		
		int insertar = arr[randomSeed]; 
		vector<int>::iterator lower; 
		int res = c-insertar;
		lower = lower_bound(T[node].begin(), T[node].end(), res+1 , greater<int>());
			
		int low = lower-T[node].begin();
		if (low == T[node].size()){
			T[node].push_back(insertar);
		}
		else{
			T[node][low] += insertar;
		}
			
		arr[randomSeed] = arr[arr.size()-1];
		arr.pop_back();
		
		sort(arr.begin(), arr.end(), greater<int>());
		firstFit(arr, node, index); 
	}
	return; 
}




int main(){
	srand(time(NULL));
	T.resize(100000);
	T[0].push_back(arr[0]);
	
	tree(0, 1, 0, 3);
	int max_iter = 5; 

		
	
	for (int i=0; i<1; i++){
		int nodo = LB[i];
		for (int iteraciones=0; iteraciones<max_iter; iteraciones++){
			firstFit(v, nodo, depth[nodo]); 
			cout << " size: " << T[nodo].size() << endl;
		}
	}
	
	
}
