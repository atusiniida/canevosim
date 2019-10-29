#include "statuscontainer.h"


StatusContainer::StatusContainer(int initialPopulationSize, int maxPopulationSize){
	this->initialPopulationSize = initialPopulationSize;
	this->maxPopulationSize = maxPopulationSize;
	container = new bool [maxPopulationSize];
	for(int i = 0; i<maxPopulationSize; i++){
		container[i] = false;
	}
	populationSize = initialPopulationSize;
}

void StatusContainer::replicate(int c){
	if(populationSize < maxPopulationSize){
 		container[populationSize] = container[c];
 		populationSize++;
 	}
}

void StatusContainer::kill(int c){
	for(int i = c+1; i < populationSize; i++){
		container[i-1] = container[i];
	}
	container[populationSize-1]  = false;
	populationSize--;
}

StatusContainer2::StatusContainer2(int initialPopulationSize, int maxPopulationSize){
	this->initialPopulationSize = initialPopulationSize;
	this->maxPopulationSize = maxPopulationSize;
	container = new int [maxPopulationSize];
	for(int i = 0; i<maxPopulationSize; i++){
		container[i] = 0;
	}
	populationSize = initialPopulationSize;
	counter = 0;
}

void StatusContainer2::replicate(int c){
	if(populationSize < maxPopulationSize){
 		container[populationSize] = container[c];
 		populationSize++;
 	}
}

void StatusContainer2::kill(int c){
	for(int i = c+1; i < populationSize; i++){
		container[i-1] = container[i];
	}
	container[populationSize-1]  = 0;
	populationSize--;
}

void StatusContainer2::set(int c, bool b){
	if(b){
		if (container[c] == 0){
			counter++;
			container[c] = counter;
		}
	}else{
		container[c]= 0;
	}
}
