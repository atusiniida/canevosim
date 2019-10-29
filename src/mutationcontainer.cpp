#include "mutationcontainer.h"



MutationContainer::MutationContainer(int initialPopulationSize, int maxPopulationSize,  int containerSize, int genomeSize, double mutationRate, double mutationFrequencyCutoff){
	this->initialPopulationSize = initialPopulationSize;
	this->maxPopulationSize = maxPopulationSize;
	this->containerSize = containerSize;
	this->mutationRate = mutationRate;
	this->genomeSize = genomeSize;
	populationSize = initialPopulationSize;
	initializeContainer();
	mt = std::mt19937(rd());
	uniform_dist = uniform_int_distribution<int>(1, genomeSize);
	poisson_dist = poisson_distribution<int>(mutationRate/2);
}

void MutationContainer::initializeContainer(){
	container = new int* [maxPopulationSize];
	counts = new int [maxPopulationSize];
	int i, j;
	for(i=0; i<maxPopulationSize; i++ ) {
		container[i] = new int [containerSize];
		counts[i] = 0;
	}
	for(i = 0; i<maxPopulationSize; i++){
		for(j = 0; j<containerSize; j++){
			container[i][j]  = 0;
		}
	}
}


void MutationContainer::freeContainer(){
	for( int i=0; i<maxPopulationSize; i++ ) {
	    delete[] container[i];
	}
	delete[] container;
	delete[] counts;
}

void MutationContainer::kill(int c){
	delete[] container[c];
	for(int i = c+1; i < populationSize; i++){
		container[i-1] = container[i];
		counts[i-1] = counts[i];
	}
	container[populationSize-1] = new int [containerSize];
	counts[populationSize-1]  = 0;
	for(int j = 0; j<containerSize; j++){
		container[populationSize-1][j]  = 0L;
	}
	populationSize--;
}


 void MutationContainer::replicate(int c){
 	if(populationSize < maxPopulationSize){
 		for(int j = 0; j<containerSize; j++){
 			container[populationSize][j]  = container[c][j];
 		}
 		counts[populationSize] = counts[c];
 		mutateRandomly(c);
 		mutateRandomly(populationSize);
 		populationSize++;
 	}
 }



void MutationContainer::mutate(int c){
	if(counts[c] < containerSize){
		container[c][counts[c]] = uniform_dist(mt);
		counts[c]++;
	}//else{
		//cerr << "error: the container reached at the limit at "<< c <<"-th cell !" << endl;
	//}
}


bool  MutationContainer::isMutated(int c, int i){
    if ( container[c][i] != 0 ){
    	 return true;
     }else{
    	 return false;
     }
}


void  MutationContainer::mutateRandomly(int c){
	int n = poisson_dist(mt);
	//int n = poisson_dist(rd);
	for(int i = 0; i < n; i++){
			mutate(c);
	}
}


string MutationContainer::toString(){
	
	string tmp = "";
	for(int i = 0; i < populationSize; i++){
		tmp = toString(i);
	}
	return tmp;
}

string MutationContainer::toString(int c){	
	if(counts[c]==0){
		return "\n";
	}else{
		string tmp = "";	
		for (int i = 0; i < counts[c]; i++){
			if(mutationFrequencyCutoff == 0 || mutationFrequency[container[c][i]] > mutationFrequencyCutoff){
				string tmp2;
				ostringstream s;
				s << container[c][i];
				tmp2 = s.str();
				tmp +=  tmp2;
				if(i != counts[c]-1){
					tmp +=  " ";
				}
			}
		}
		tmp += "\n";
		return tmp;
	}	
}


void MutationContainer::calculateMutationFrequency(){
	for(int c = 0; c < populationSize; c++){
		for (int i = 0; i < counts[c]; i++){
			auto it = mutationFrequency.find(container[c][i]);
	  		if (it != mutationFrequency.end()){
				it->second += 1.0;
	  		}else{
				mutationFrequency.insert(pair<int, double>(container[c][i], 1.0));
	  		}
		}
	}
	for (auto it = mutationFrequency.begin(); it != mutationFrequency.end(); ++it) {
		it->second /= populationSize;
	}

}

