#include "tumor.h"
#include <iostream>
#include <memory>
#include <vector>
#include <unistd.h>
#include <fstream>

using namespace std;


double baseReplicationRate =  0.0001;
double baseDeathRate = 0;
int driverMutationContainerSize = 5;
int driverGenomeSize = 10000;
double driverMutationRate = 0.01;
int passengerMutationContainerSize = 1000;
int passengerGenomeSize = 1000000;
double passengerMutationRate  = 0.1;
double replicationRateIncrease = 5;
double deathRateDecrease = 1;
int maxTime = 1000000;
int maxPopulationSize = 100000;
int initialPopulationSize = 1;
int tumorSampleSize = 1000;
double mutationFrequencyCutoff = 0;
double populationSizeChangeCutoff = 0;
double carryingCapacity = INFINITY;
double symmetricReplicationProbablity = 1;
double infinitizingCarryingCapacityProbability = 0;
double differentiatedCellDeathRate = 0.01;
string outfilePrefix = "out";

	void printUsage()
{
    printf("usage: python bep.py [option]\n");
		printf("option:\n");
    printf("  -r baseReplicationRate(%f)\n", baseReplicationRate);
    printf("  -d baseDeathRate(%f)\n", baseDeathRate);
    printf("  -c driverMutationContainerSize(%d)\n", driverMutationContainerSize);
	printf("  -g driverGenomeSize(%d)\n", driverGenomeSize);
	printf("  -m driverMutationRate(%f)\n", driverMutationRate);
	printf("  -C passengerMutationContainerSize(%d)\n", passengerMutationContainerSize);
	printf("  -G passengerGenomeSize(%d)\n", passengerGenomeSize);
	printf("  -M passengerMutationRate(%f)\n", passengerMutationRate);
	printf("  -f replicationRateIncrease(%f)\n", replicationRateIncrease);
	printf("  -D deathRateDecrease(%f)\n", deathRateDecrease);
	printf("  -T maxTime(%d)\n", maxTime);
	printf("  -P maxPopulationSize(%d)\n", maxPopulationSize);
	printf("  -p initialPopulationSize(%d)\n", initialPopulationSize);
	printf("  -s tumorSampleSize(%d)\n", tumorSampleSize);
	printf("  -F mutationFrequencyCutoff(%f)\n", mutationFrequencyCutoff);
	printf("  -k populationSizeChangeCutoff (%f, if positive, print time course data)\n", populationSizeChangeCutoff );
	printf("  -K carryingCapacity(%f)\n", carryingCapacity);
	printf("  -S symmetricReplicationProbablity (%f)\n", symmetricReplicationProbablity );
	printf("  -e differentiatedCellDeathRate (%f)\n", differentiatedCellDeathRate );
	printf("  -i infinitizingCarryingCapacityProbability (%f)\n", infinitizingCarryingCapacityProbability);
	printf("  -o outfilePrefix(%s)\n", outfilePrefix.c_str());

}


int main(int argc, char* argv[]){


    int  c, option_index;

    while (1){

	  c = getopt(argc, argv, "r:d:c:g:m:C:G:M:f:D:T:P:p:s:F:k:K:S:e:i:o:h");
	  if ( c == -1 ) break;

	  switch(c){
	  case 'r': baseReplicationRate               = atof(optarg); break;
	  case 'd': baseDeathRate                     = atof(optarg); break;
	  case 'c': driverMutationContainerSize       = atoi(optarg); break;
	  case 'g': driverGenomeSize                  = atoi(optarg); break;
	  case 'm': driverMutationRate                = atof(optarg); break;
	  case 'C': passengerMutationContainerSize    = atoi(optarg); break;
	  case 'G': passengerGenomeSize               = atoi(optarg); break;
	  case 'M': passengerMutationRate             = atof(optarg); break;
	  case 'f': replicationRateIncrease                   = atof(optarg); break;
	  case 'D': deathRateDecrease                  = atof(optarg); break;
	  case 'T': maxTime                           = atoi(optarg); break;
	  case 'P': maxPopulationSize                 = atoi(optarg); break;
	  case 'p': initialPopulationSize             = atoi(optarg); break;
	  case 's': tumorSampleSize                   = atoi(optarg); break;
	  case 'F': mutationFrequencyCutoff           = atof(optarg); break;
	  case 'k': populationSizeChangeCutoff        = atof(optarg); break;
	  case 'K': carryingCapacity                  = atof(optarg); break;
	  case 'S': symmetricReplicationProbablity    = atof(optarg); break;
	  case 'e': differentiatedCellDeathRate       = atof(optarg); break;
	  case 'i': infinitizingCarryingCapacityProbability     = atof(optarg); break;
	  case 'o': outfilePrefix                     = optarg; break;
	  case 'h': printUsage(); exit(0);

	  case '?':
	       fprintf(stderr,"BEP_err: Illegal option %s is recognized.\n",argv[optind]);
	       exit(EXIT_FAILURE);

	  case ':':
	       fprintf(stderr,"BEP_err: Option %s requires a value.\n", argv[optind]);
	       exit(EXIT_FAILURE);

	  default:
		  printUsage();
		  exit(0);
	  }

    }

	Tumor T = Tumor(baseReplicationRate,
		baseDeathRate,
		driverMutationContainerSize,
  		driverGenomeSize,
  		driverMutationRate,
  		passengerMutationContainerSize,
  		passengerGenomeSize,
  		passengerMutationRate,
		replicationRateIncrease,
		deathRateDecrease,
		maxTime,
		maxPopulationSize,
		initialPopulationSize,
		tumorSampleSize,
		mutationFrequencyCutoff,
		carryingCapacity,
		symmetricReplicationProbablity,
		differentiatedCellDeathRate,
		infinitizingCarryingCapacityProbability
		);

	if(populationSizeChangeCutoff > 0){
		T.simulateWhilePrinting(outfilePrefix, populationSizeChangeCutoff);
	}else{
		T.simulate();
		T.printResults(outfilePrefix);
	}
	return 0;
}
