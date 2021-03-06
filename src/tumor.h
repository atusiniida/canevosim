#ifndef TUMOR_H_
#define TUMOR_H_

#include <string>
#include <cmath>
#include <float.h>
#include <signal.h>
#include <time.h>
#include "tumor.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include "mutationcontainer.h"
#include "statuscontainer.h"

using namespace std;

class Tumor {
	public:
		Tumor(
				double baseReplicationRate,
				double baseDeathRate,
				int driverMutationContainerSize,
		  		int driverGenomeSize,
		  		double driverMutationRate,
		  		int passengerMutationContainerSize,
		  		int passengerGenomeSize,
		  		double passengerMutationRate,
				double replicationRateIncrease,
				double deathRateDecrease,
				int maxTime,
				int maxPopulationSize,
				int initialPopulationSize,
				int tumorSampleSize,
				double mutationFrequencyCutoff,
				double carryingCapacity,
				double symmetricReplicationProbablity,
				double differentiatedCellDeathRate,
				double infinitizingCarryingCapacityProbability
				);

		virtual ~Tumor();
		void simulate();
		void printResults(string outfilePrefix);
		void simulateWhilePrinting(string outfilePrefix, double k);
	private:
		double baseReplicationRate;
		double baseDeathRate;
		int driverMutationContainerSize;
		int driverGenomeSize;
		double driverMutationRate;
		int passengerMutationContainerSize;
		int passengerGenomeSize;
		double passengerMutationRate;
		double replicationRateIncrease;
		double deathRateDecrease;
		int maxTime;
		int maxPopulationSize;
		int initialPopulationSize;
		int tumorSampleSize;
		double mutationFrequencyCutoff;
		double carryingCapacity ;
		double symmetricReplicationProbablity;
		double differentiatedCellDeathRate;
		double infinitizingCarryingCapacityProbability;

		MutationContainer*  driverMutationContainer;
		MutationContainer*  passengerMutationContainer;
		StatusContainer*  differentiation;
		StatusContainer2*  infiniteCarryingCapacity;

		int time;
		int populationSize;
		double calculationTime;

		double getGrowthRate(int i);
		double getDeathRate(int i);
		bool tryKill(int c);
		void kill(int c);
		bool tryReplicate(int c);
		bool update();
		void replicateSymmetrically(int c);
		void replicateAsymmetrically(int c);
		string getParameterString();
		vector<int> sampleCells();

		void replicateInfiniteCarryingCapacity(int i);


};

#endif /* TUMOR_H_ */
