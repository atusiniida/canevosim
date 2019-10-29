#ifndef MUTATIONCONTAINER_H_
#define MUTATIONCONTAINER_H_

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include "random.h"
using namespace std;


class MutationContainer {
public:
	MutationContainer(int initialPopulationSize, int maxPopulationSize,  int containerSize, int genomeSize, double mutationRate, double mutationFrequencyCutoff);
	virtual ~MutationContainer(){freeContainer();};

	int countMutations(int c){return counts[c];};
	void kill(int c);
	void replicate(int c);
	string toString();
	string toString(int c);
	void calculateMutationFrequency();
	int size(){return populationSize;};

private:
	int initialPopulationSize;
	int maxPopulationSize;
	int populationSize;
	int genomeSize;
	int containerSize;
	int **container;
	int *counts;
	double mutationFrequencyCutoff;

	void initializeContainer();
	void initializeRand();
	void freeContainer();
	void mutate(int c);
	void mutateRandomly(int c);
	bool isMutated(int c, int i);
	
	double mutationRate;
	map <int, double> mutationFrequency;


	std::random_device rd;
	std::mt19937 mt;
	uniform_int_distribution<int> uniform_dist;
	poisson_distribution<int> poisson_dist;
	
};

#endif /*MUTATIONCONTAINER_H_*/
