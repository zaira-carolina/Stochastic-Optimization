#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/time.h>
#include <iostream>
#include <signal.h>
#include <cmath>

#include "leerArchivo.h"
#include "auxiliares.h"
#include "individuo.h"

#define MAX_DISTANCE LLONG_MAX

using namespace std; 
int N = 50; // population
int size, maxCapacity; // elementos por acomodar
double pc = 0.89, finalTime, initialTime, elapsedTime, DI; 
int MAX_SOL = size;
int *arr = NULL;
vector<Individual> population, parents, offspring; 
int openBins = 35, nElitista = 5;
vector<vector<int>> res; 



void print(Individual p){
	int count = 0;
	int suma; 
	for (int i=0; i<p.elements.size(); i++){
		cout << "i: " << i << " ";
		suma = 0;
		for (int j=0; j<p.elements[i].size(); j++){
			cout << p.elements[i][j] << " " ;
			count ++;
			suma += arr[p.elements[i][j]];
		}
		cout << " s: " << suma << endl;
	}	
	cout << "total: " << count << endl;
}

void Heuristic(Individual &p){
	p.elements.resize(size);
	// revolver los elementos por acomodar
	int index[size];
	for (int i=0; i<size; i++){
		index[i] = i;
	}
	// random shuffle
	random_shuffle(&index[0], &index[size-1]);
	int suma = 0, bins = 0;
	
	int i = 0;
	while (i < size){
		suma += arr[ index[i] ];
		if (suma > maxCapacity){
			suma = 0;
			bins ++;
			i--;
		} else{
			p.elements[bins].push_back(index[i]); 
			p.elementInBin.push_back ( make_pair( index[i], bins ) ); 
		}
		i++;
	}
	p.nbins = bins;

}

void fitness(Individual &p){
	int n = p.nbins, sumaElementos; 
	double sumaTotal = 0;
	for (int i=0; i<n; i++){
		sumaElementos = 0;
		for (int j=0; j<p.elements[i].size(); j++){
			if ( p.elements[i].size() != 0){
				sumaElementos += p.elements[i][j];
			}
		}
		sumaTotal += ((sumaElementos / (double)maxCapacity))*((sumaElementos / (double)maxCapacity));
	}
	double f;
	f = 1.0-(sumaTotal / (double)n);
	p.fitness = f; 
}

void initPopulation(){
	population.clear();
	for (int i=0; i<N; i++){
		Individual p;
		Heuristic(p); 
		fitness(p); 
		population.push_back(p); 
	}
}

void selectParents(){
	parents.clear();
	for (int i=0; i<N; i++){
		int first = getRandomInteger0_N(N-1);
		int second = getRandomInteger0_N(N-1); 
		if (population[first].fitness <= population[second].fitness){
			parents.push_back(population[first]);
		} else{
			parents.push_back(population[second]);
		}
	}
}

void crossFunction(Individual &p, Individual &q){
	// Funcion de crossover de dos puntos
	// Ambos padres se actualizan por cada hijo
	int p1, p2, q1, q2;
	// CROSSOVER POINTS
	p1 = getRandomInteger0_N(p.nbins); p2 = getRandomInteger0_N(p.nbins);
	q1 = getRandomInteger0_N(q.nbins); q2 = getRandomInteger0_N(q.nbins);
	
	int ap, bp, aq, bq; // cortes
	// corte en padre 1
	if (p1 < p2)
		ap = p1, bp = p2;
	else
		ap = p2, bp = p1;
	// corte en padre 2
	if (q1 < q2)
		aq = q1, bq = q2;
	else
		aq = q2, bq = q1;
	// hacer las combinaciones
	// PRIMERO BORRAR LOS ELEMENTOS QUE VAMOS A PONER EXTRA EN P
	vector<int> borrarEnP;
	vector<vector<int>> InsertarEnP (bq-aq);
	
	int n = 0;
	//cout << "Por borrar en p" << endl;
	for (int i=aq; i<bq; i++){
		for (int j=0; j<q.elements[i].size(); j++){
			InsertarEnP[n].push_back( q.elements[i][j] );
			borrarEnP.push_back( q.elements[i][j] );
			//cout << q.elements[i][j] << " ";
		}
		n++;
	}
	//cout << "\n";
	vector<int> borrarEnQ; 
	vector<vector<int>> InsertarEnQ (bp-ap);
	n = 0; 
	for (int i=ap; i<bp; i++){
		for (int j=0; j<p.elements[i].size(); j++){
			InsertarEnQ[n].push_back( p.elements[i][j] );
			borrarEnQ.push_back( p.elements[i][j] );
		}
		n++;
	}
	// ¿donde estan?
	sort(p.elementInBin.begin(), p.elementInBin.end(), [&](auto a, auto b){return a.first< b.first;});
	vector<int> borrarEnBinP;
	for (int i=0; i<borrarEnP.size(); i++){
		borrarEnBinP.push_back( p.elementInBin[ borrarEnP[i] ].second );
	}
	// BORRAR LOS ELEMENTOS DE P
	for (int i=0; i<borrarEnBinP.size(); i++){
		//cout << "elemento por borrar: " << borrarEnP[i] << " - ";
		for (int j=0; j<p.elements[borrarEnBinP[i]].size(); j++){
			//cout << p.elements[borrarEnBinP[i]][j] << " ";
			if (p.elements[borrarEnBinP[i]][j] == borrarEnP[i]){
				p.elements[borrarEnBinP[i]][j] = p.elements[borrarEnBinP[i]].back();
				p.elements[borrarEnBinP[i]].pop_back();
			}
		}
		//cout << endl;
	}
	// BORRAR LOS ELEMENTOS DE Q
	sort(q.elementInBin.begin(), q.elementInBin.end(), [&](auto a, auto b){return a.first< b.first;});
	vector<int> borrarEnBinQ;
	for (int i=0; i<borrarEnQ.size(); i++){
		borrarEnBinQ.push_back( q.elementInBin[ borrarEnQ[i] ].second );
	}
	
	for (int i=0; i<borrarEnBinQ.size(); i++){
		for (int j=0; j<q.elements[borrarEnBinQ[i]].size(); j++){
			if (q.elements[borrarEnBinQ[i]][j] == borrarEnQ[i]){
				q.elements[borrarEnBinQ[i]][j] = q.elements[borrarEnBinQ[i]].back();
				q.elements[borrarEnBinQ[i]].pop_back();
			}
		}
	}
	// AHORA SI, PASAR LOS DATOS
	
	vector<vector<int>> newP (2*size);
	// PASAR LOS PRIMEROS DATOS DE P
	for (int i=0; i<ap; i++){
		for (int j=0; j<p.elements[i].size(); j++){
			newP[i].push_back(p.elements[i][j]);
		}
	}
	// PASAR LOS DATOS DE Q
	int element = 0;
	for (int i=ap; i<(ap+InsertarEnP.size()); i++){
		for (int j=0; j<InsertarEnP[element].size(); j++){
			newP[i].push_back( InsertarEnP[element][j] );
		}
		element++;
	}
	
	element = ap+InsertarEnP.size();
	// PASAR LOS ULTIMOS DATOS DE P
	for (int i=ap; i<size; i++){
		for (int j=0; j<p.elements[i].size(); j++){
			newP[element].push_back( p.elements[i][j] );
		}
		element++; 
	}
	// PASAR LOS DATOS DE Q
	vector<vector<int>> newQ (2*size);
	vector<int> deleteInQ; 
	for (int i=0; i<aq; i++){
		for (int j=0; j<q.elements[i].size(); j++){
			newQ[i].push_back( q.elements[i][j] );
		}
	}
	// PASAR DATOS DE P A Q
	element = 0;
	for (int i=aq; i<(aq+InsertarEnQ.size()); i++){
		for (int j=0; j<InsertarEnQ[element].size(); j++){
			newQ[i].push_back( InsertarEnQ[element][j] );
		}
		element++;
	}

	// PASAR ULTIMOS DATOS DE Q
	element = aq + InsertarEnQ.size();
	for (int i=aq; i<size; i++){
		for (int j=0; j<q.elements[i].size(); j++){
			newQ[element].push_back( q.elements[i][j] );
		}
		element++; 
	}
	// ACTUALIZAR P 
	p = {};
	p.elements.resize(size);
	n = 0;
	int index = 0;
	for (int i=0; i<newP.size(); i++){
		if (newP[i].size() != 0){
			for (int j=0; j<newP[i].size(); j++){
				p.elements[n].push_back(newP[i][j]);
				p.elementInBin.push_back(make_pair(newP[i][j], n));
				index++;
			}
			n++;
		}
	}
	
	
	p.nbins = n; 
	fitness(p);

	/*
	cout << "Ver p: " << endl;
	for (int i=0; i<size; i++){
		cout << p.elementInBin[i].first << " " << p.elementInBin[i].second << endl;
	}*/

	
	
	// ACTUALIZAR Q
	q = {};
	q.elements.resize(size);
	n = 0, index = 0;
	for (int i=0; i<newQ.size(); i++){
		if (newQ[i].size() != 0){
			for (int j=0; j<newQ[i].size(); j++){
				q.elements[n].push_back(newQ[i][j]);
				q.elementInBin.push_back( make_pair(newQ[i][j], n) );
				index++;
			}
			n++;
		}
	}
		
	q.nbins = n;
	fitness(q);
}

void crossover(){
	offspring.clear(); 
	for (int i=0; i<parents.size(); i++){
		offspring.push_back(parents[i]);
	}
	
	for (int i=0; i<offspring.size(); i+=2){
		if (generateRandomDouble0_Max(1) <= pc){
			crossFunction(offspring[i], offspring[i+1]);
		}
	}
}



void LocalSearch(Individual &p){
	/*The n least full bins are opened and their contents are made free.
	 * Items in the remaining bins are replaced by larger free items. */
	int s;
	vector<pair<int, int>> sumas;
	for (int i=0; i<p.elements.size(); i++){
		s = 0;
		if (p.elements[i].size() != 0){
			for (int j=0; j<p.elements[i].size(); j++){
				s += arr[p.elements[i][j]];
			}	
			sumas.push_back( make_pair(s, i) );
		}
	}
	
	//cout << "BEFORE LOCAL SEARCH: " << endl;
	//print(p); 
	
	
	// Acomodar en orden para tomar los n mas chicos para abrirlos
	sort(sumas.begin(), sumas.end());
	vector<int> binsToOpen, elementsToMove;
	// BINS POR ABRIR
	for (int i=0; i<openBins; i++){
		binsToOpen.push_back(sumas[i].second);
	}
	// ELEMENTOS POR MOVER
	//cout << "elementos por borrar" << endl;
	for (int i=0; i<binsToOpen.size(); i++){
		for (int j=0; j<p.elements[ binsToOpen[i] ].size(); j++){
			elementsToMove.push_back(  p.elements[binsToOpen[i]][j] );
			//cout << p.elements[binsToOpen[i]][j] << " ";
		}
	}
	// DELETE BINS FROM P
	//cout << "\n binsDelte" << endl;
	for (int i=0; i<binsToOpen.size(); i++){
		p.elements[ binsToOpen[i] ].clear();
		//cout << binsToOpen[i] << " ";
	}
	
	// vaciar
	//cout << endl;
	
	vector<int> contenedoresVacios;
	for (int i=0; i<sumas.size(); i++){
		for (int j=0; j<binsToOpen.size(); j++){
			if (sumas[i].second == binsToOpen[j]){
				sumas[i].first = -1;
				contenedoresVacios.push_back(i); 
			}
		}
	}

	
	// ADD ELEMENTS BY FIRST FIT 
	int element = 0, penalization = 0, newBin = 0, c = 1;
	int n = elementsToMove.size(); 
	while ( element  < n ){
		penalization = 0;
		for (int i=0; i<sumas.size(); i++){
			if ( sumas[i].first != -1 && ( (arr[elementsToMove[element]] + sumas[i].first) <= maxCapacity) ){
				p.elements[ sumas[i].second ].push_back( elementsToMove[element] );
				p.elementInBin[elementsToMove[element]].second = sumas[i].second; 
				
				sumas[i].first += arr[elementsToMove[element]];				
				penalization = -1;
				break;
			}
		}

		//cout << "quedan " << endl;
		//cout << elementsToMove[element] << " " << penalization << " nbins: " << p.nbins << endl; 

		if (penalization == 0){
			for (int i=0; i<sumas.size(); i++){
				if (sumas[i].first == -1 || sumas[i].first == 0){

					p.elements[ sumas[i].second ].push_back( elementsToMove[element] );
					p.elementInBin[elementsToMove[element]].second = sumas[i].second; 
				
					sumas[i].first += arr[elementsToMove[element]];				
					penalization = -1;
					break;
				}
			}
		}
		element++;
	}

	
	// HACER COPIA DE P 
	Individual copyP = p;
	p = {};	
	p.elements.resize(size);
	
	n = 0;
	int index = 0;
	for (int i=0; i<copyP.elements.size(); i++){
		if (copyP.elements[i].size() != 0){
			for (int j=0; j<copyP.elements[i].size(); j++){
				p.elements[n].push_back( copyP.elements[i][j] );
				p.elementInBin.push_back( make_pair(copyP.elements[i][j], n) );
				index++;
			}
			n++;
		}
	}
	// ERROR, ESTAN SALIENDO MENOS DEL OPTIMO
	//cout << "n: " << n << endl; 
	p.nbins = n;
	fitness(p);
	
	//cout << "despues de Local Search" << endl;
	//print(p); 
	
}

void intensify(){
	for (int i=0; i<offspring.size(); i++){
		//cout << "intensify: " << i << endl;
		LocalSearch(offspring[i]); 
	}
}


double getDistance(Individual &p, Individual &q){
	double x1, x2, y1, y2;
	
	x1 = (double)p.nbins; x2 = (double)q.nbins;
	
	int sumaP =  0;
	for (int i=0; i<p.nbins; i++){
		int s = 0;
		for (int j=0; j<p.elements[i].size(); j++){
			s += arr[ p.elements[i][j] ];
		}
		// free space in each bin
		sumaP += (maxCapacity-s);
	}
	y1 = sumaP; 
	
	
	int sumaQ = 0;
	for (int i=0; i<q.nbins; i++){
		int s = 0;
		for (int j=0; j<q.elements[i].size(); j++){
			s += arr[ q.elements[i][j] ];  
		}
		sumaQ += (maxCapacity-s);
	}
	y2 = sumaQ;
	
	
	double d;
	d = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
	return sqrt(d);
}


void replacement(){
	vector<Individual> all;
	for (int i=0; i<population.size(); i++){
		all.push_back(population[i]);
		all.back().distance = MAX_DISTANCE;
	}
	population.clear();
	
	for (int i=0; i<offspring.size(); i++){
		all.push_back(offspring[i]);
		all.back().distance = MAX_DISTANCE;
	}
	
	offspring.clear();
	int indexBest = 0;
	for (int i=1; i<all.size(); i++){
		if (all[i].fitness < all[indexBest].fitness){
			indexBest = i;
		}
	}
	
	population.push_back(all[indexBest]);
	all[indexBest] = all.back();
	all.pop_back();
	
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	double elapsedTime = (double)(currentTime.tv_sec)+(double)(currentTime.tv_usec)/1.0e6;
	double D = DI-DI*elapsedTime/finalTime;
	
	while (population.size() != N){
		
		for (int i=0; i<all.size(); i++){
			all[i].distance = min(all[i].distance,  getDistance(all[i], population.back()) );
		}
		
		indexBest = 0;
		for (int i=1; i<all.size(); i++){
			bool betterInDist = (all[i].distance > all[indexBest].distance);
			bool eqInDist = (all[i].distance == all[indexBest].distance);
			bool betterInFit = (all[i].fitness < all[indexBest].fitness);
			bool eqInFit = (all[i].fitness == all[indexBest].fitness);
		
			if (all[indexBest].distance < D){
				if ((betterInDist) || (eqInDist && betterInFit)){
					indexBest = i;
				}
			} else {
				if (all[i].distance >= D){
					if ((betterInFit) || (eqInFit && betterInDist)){
						indexBest = i;
					}
				}
			}
		}
		
		population.push_back(all[indexBest]);
		all[indexBest] = all.back();
		all.pop_back();
	}
}

void initDI(){
	double meanDistance = 0;
	for (int i=0; i<population.size(); i++){
		for (int j=i+1; j<population.size(); j++){
			meanDistance += getDistance(population[i], population[j]);
		}
	}
	
	meanDistance /= (population.size()*(population.size()-1))/2;
	DI = meanDistance*0.5;
}


// con reemplazamiento elitista


void replacementEli(){

	vector<Individual> all;
	for (int i=0; i<population.size(); i++){
		all.push_back(population[i]); 
	}
	population.clear();
	
	for (int i=0; i<offspring.size(); i++){
		all.push_back(offspring[i]);
	}
	offspring.clear();
	
	int indexBest = 0;
	for (int i=1; i<all.size(); i++){
		if (all[i].fitness < all[indexBest].fitness){
			indexBest = i;
		}
	}
	
	//cout << "here" << endl;
	population.push_back(all[indexBest]);
	all[indexBest] = all.back();
	all.pop_back();
	
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	double elapsedTime = (double)(currentTime.tv_sec)+(double)(currentTime.tv_usec)/1.0e6;
	elapsedTime -= initialTime; 
	
	while (population.size() != N){
		indexBest = 0;
		for (int i=1; i<all.size(); i++){
			if (all[i].fitness < all[indexBest].fitness){
				indexBest = i;
			}
		}
		population.push_back(all[indexBest]);
		all[indexBest] = all.back();
		all.pop_back();
	}
}


void run(){
	initPopulation();
	initDI();
	int generation = 0;
	double cTime; 
	double bestFit;
	int bestBins; 
	res.resize(size); 
	
	do{
		cout << "generation: " << generation << endl;
		srand( ( ( ( long long ) ( rand ( ) ) << 32 ) ^ rand ( ) ));
		
		//srand(time(NULL)*generation); 
		selectParents();
		crossover();
		intensify();
		replacement();
		struct timeval currentTime; 
		gettimeofday(&currentTime, NULL);
		cTime = (double)(currentTime.tv_sec)+(double)(currentTime.tv_usec)/1.0e6;
		elapsedTime = cTime-initialTime;
		
		if (generation == 0){
			bestFit = population[0].fitness;
			bestBins = population[0].nbins;

			for (int i=0; i<bestBins; i++){
				for (int j=0; j<population[0].elements[i].size(); j++){
					res[i].push_back(population[0].elements[i][j]);
				}
			}



		} else{
			if (population[0].fitness < bestFit){
				bestFit = population[0].fitness;
			}

			if (population[0].nbins < bestBins){
				bestBins = population[0].nbins;
				
				res.clear();
				res.resize(size);
				for (int i=0; i<bestBins; i++){
					for (int j=0; j<population[0].elements[i].size(); j++){
						res[i].push_back(population[0].elements[i][j]);
					}
				}



			}

		}

		cout << bestBins << endl;

		generation++;
	} while (generation < 200);
}

int main(){
	//srand(time(NULL)); 
	int maxCapacityOne;
	char instancia1[30] = "Falkenauer_u120_14.txt";
	arr = readFile(instancia1, &size, &maxCapacityOne);
	

	maxCapacity = maxCapacityOne-1; 
	struct timeval currentTime; 
	gettimeofday(&currentTime, NULL);
	
	initialTime = (double)(currentTime.tv_sec)+(double)(currentTime.tv_usec)/1.0e6;
	finalTime = 30; 
	
	run();

	int ss;
	for (int i=0; i<res.size(); i++){
		cout << "i: " << i << " ";
		ss = 0;
		for (int j=0; j<res[i].size(); j++){
			cout << res[i][j] << " ";
			ss += arr[res[i][j]];
		}
		cout << " s: " << ss << endl;
	}


}
