#include "tumor.h"


Tumor::Tumor(
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
		){
	initRand();
	this->baseReplicationRate = baseReplicationRate;
	this->baseDeathRate = baseDeathRate;
	this->maxTime = maxTime;
	this->maxPopulationSize =  maxPopulationSize;
	this->initialPopulationSize = initialPopulationSize;
	this->replicationRateIncrease = replicationRateIncrease;
	this->deathRateDecrease = deathRateDecrease;
	this->tumorSampleSize = tumorSampleSize;
	this->mutationFrequencyCutoff =  mutationFrequencyCutoff;
	this->driverMutationContainerSize = driverMutationContainerSize;
  	this->driverGenomeSize =  driverGenomeSize;
  	this->driverMutationRate =  driverMutationRate;
  	this->passengerMutationContainerSize = passengerMutationContainerSize;
  	this->passengerGenomeSize = passengerGenomeSize;
  	this->passengerMutationRate = passengerMutationRate;
  	this->carryingCapacity = carryingCapacity;
  	this->symmetricReplicationProbablity = symmetricReplicationProbablity;
  	this->differentiatedCellDeathRate = differentiatedCellDeathRate;
  	this->infinitizingCarryingCapacityProbability = infinitizingCarryingCapacityProbability;

	time = 0;
	populationSize = initialPopulationSize;
	driverMutationContainer = new  MutationContainer(initialPopulationSize, maxPopulationSize, driverMutationContainerSize, driverGenomeSize, driverMutationRate, mutationFrequencyCutoff);
	passengerMutationContainer = new  MutationContainer(initialPopulationSize, maxPopulationSize, passengerMutationContainerSize, passengerGenomeSize, passengerMutationRate, mutationFrequencyCutoff);
	differentiation  = new StatusContainer(initialPopulationSize, maxPopulationSize);
	infiniteCarryingCapacity  = new StatusContainer2(initialPopulationSize, maxPopulationSize);
}


Tumor::~Tumor(){
	//cout << "~Tumor" << endl;
	delete driverMutationContainer;
	delete passengerMutationContainer;
	delete differentiation;
	delete infiniteCarryingCapacity;
}

double Tumor::getGrowthRate(int i){
	int k = driverMutationContainer->countMutations(i);
	double fittness = pow(replicationRateIncrease,k);
	if(!std::isinf(carryingCapacity) & infiniteCarryingCapacity->get(i)== 0) {
		fittness *= 1 - populationSize/carryingCapacity;
	}
	return baseReplicationRate * fittness;
}

double Tumor::getDeathRate(int i){
	int k = driverMutationContainer->countMutations(i);
	double fittness = pow(deathRateDecrease,k);
	if(!std::isinf(carryingCapacity) & infiniteCarryingCapacity->get(i) == 0) {
		fittness *= 1 - populationSize/carryingCapacity;
	}
	if (differentiation->get(i)){
		return differentiatedCellDeathRate / fittness;
	}else{
		return baseDeathRate / fittness;
	}
}

// bool Tumor::replicate(){
// 	int tmp = populationSize;
// 	bool update = false;
// 	for(int i = 0; i < tmp; i++){
// 		if(populationSize == maxPopulationSize){
// 			break;
// 		}
// 		double d = getGrowthRate(i);
// 		if( (d < 1 &  d > doubleRand()) | (d >= 1)) {
// 			if((symmetricReplicationProbablity >=  1 )| differentiation->get(i) | (symmetricReplicationProbablity > doubleRand()) ){
// 				replicateSymmetrically(i);
// 			}else{
// 				replicateAsymmetrically(i);
// 			}
// 			update = true;
// 		}
// 	}
// 	return update ;
// }

bool Tumor::update(){
	int n = populationSize;
	bool update = false;
	for(int i = 0; i < n; i++){
		if(populationSize == maxPopulationSize){
			break;
		}
		if(boolRand()){
			if(tryReplicate(i)){
				update = true;
			}else if(tryKill(i)){
				update = true;
				i--;
				n--;
			}
		}else{
			if(tryKill(i)){
				update = true;
				i--;
				n--;
			}else if(tryReplicate(i)){
				update = true;
			}
		}
	}
	return update ;
}

bool Tumor::tryReplicate(int c){
	double d = getGrowthRate(c);
	if( (d < 1 &  d > doubleRand()) | (d >= 1)) {
		if((symmetricReplicationProbablity >=  1 )| differentiation->get(c) | (symmetricReplicationProbablity > doubleRand()) ){
			replicateSymmetrically(c);
		}else{
			replicateAsymmetrically(c);
		}
		return true;
	}else{
		return false;
	}
}

void Tumor::replicateAsymmetrically(int c){
	driverMutationContainer->replicate(c);
	passengerMutationContainer->replicate(c);
	differentiation->replicate(c);
	differentiation->set(populationSize, true);
	replicateInfiniteCarryingCapacity(c);
	populationSize++;
}

void Tumor::replicateSymmetrically(int c){
	driverMutationContainer->replicate(c);
	passengerMutationContainer->replicate(c);
	differentiation->replicate(c);
	replicateInfiniteCarryingCapacity(c);
	populationSize++;
};

void Tumor::replicateInfiniteCarryingCapacity(int c){
	infiniteCarryingCapacity->replicate(c);
	if(infinitizingCarryingCapacityProbability > 0){
		if(driverMutationContainer->countMutations(c)==driverMutationContainerSize && infinitizingCarryingCapacityProbability/2 > doubleRand()){
			infiniteCarryingCapacity->set(c, true);

		}
		int n = infiniteCarryingCapacity->size() -1;
		if(driverMutationContainer->countMutations(n)==driverMutationContainerSize && infinitizingCarryingCapacityProbability/2 > doubleRand()){
			infiniteCarryingCapacity->set(n, true);
		}
	}
}

// bool Tumor::kill(){
// 	int tmp = populationSize;
// 	bool update = false;
// 	for(int i = tmp-1; i >= 0; i--){
// 		double d = getDeathRate(i);
// 		if(d != 0 & d > doubleRand()){
// 			kill(i);
// 			update = true;
// 		}
// 	}
// 	return update;
// }


bool Tumor::tryKill(int c){
	double d = getDeathRate(c);
	// if(d != 0 & d > doubleRand()){
	if(populationSize > 1 & d != 0 & d > doubleRand()){
		kill(c);
		return true;
	}else{
		return false;
	}
}

void Tumor::kill(int c){
	driverMutationContainer->kill(c);
	passengerMutationContainer->kill(c);
	differentiation->kill(c);
	infiniteCarryingCapacity->kill(c);
	populationSize--;
}

static int s = 0;
void sigcatch(int sig) {
	s = 1;
}



void Tumor::simulate(){
	clock_t start = clock();
	int d = 100;
	int tmp =  populationSize;
	if (SIG_ERR == signal(SIGINT, sigcatch)) {
		printf("failed to set signal handler!");
		exit(1);
	}

	int unupdated = 0;
	int unupdatedCutoff = 100;

	 for(time = 1; time <= maxTime; time++){
		 if(update()){
		 	unupdated = 0;
		 }else{
		 	unupdated++;
		 }
		 if(fabs(time/d - (double)time/d) < LDBL_EPSILON){
			 cout << "time=" << time <<"\t" << "populationSize=" <<  populationSize << endl;
			 tmp =  populationSize;

		}
		if(populationSize == 0 | populationSize >= maxPopulationSize){
			 cout << "time=" << time <<"\t" << "populationSize=" <<  populationSize << endl;

			 clock_t end = clock();
			 calculationTime = (double)(end - start) / CLOCKS_PER_SEC;
			 break;
		}
		if (s == 1){
			clock_t end = clock();
			calculationTime = (double)(end - start) / CLOCKS_PER_SEC;
			break;
		}
		if((populationSize == carryingCapacity) &  (unupdated == unupdatedCutoff)){
			clock_t end = clock();
			calculationTime = (double)(end - start) / CLOCKS_PER_SEC;
			break;
		}
	}
}


string Tumor::getParameterString(){
	ostringstream s;

	s << "baseReplicationRate=" << baseReplicationRate << "\n";
	s << "baseDeathRate=" << baseDeathRate << "\n";
	s << "driverMutationContainerSize=" << driverMutationContainerSize << "\n";
	s << "driverGenomeSize=" << driverGenomeSize << "\n";
	s << "driverMutationRate=" << driverMutationRate << "\n";
	s << "passengerMutationContainerSize=" << passengerMutationContainerSize << "\n";
	s << "passengerGenomeSize=" << passengerGenomeSize << "\n";
	s << "passengerMutationRate=" << passengerMutationRate << "\n";
	s << "replicationRateIncrease=" << replicationRateIncrease << "\n";
	s << "deathRateDecrease=" << deathRateDecrease << "\n";
	s << "maxTime=" << maxTime << "\n";
	s << "maxPopulationSize=" << maxPopulationSize << "\n";
	s << "initialPopulationSize=" << initialPopulationSize << "\n";
	s << "carryingCapacity=" << carryingCapacity << "\n";
	s << "symmetricReplicationProbablity=" << symmetricReplicationProbablity << "\n";
	s << "differentiatedCellDeathRate=" << differentiatedCellDeathRate << "\n";
	s << "infinitizingCarryingCapacityProbability=" << infinitizingCarryingCapacityProbability << "\n";
	s << "tumorSampleSize=" << tumorSampleSize << "\n";
	s << "mutationFrequencyCutoff=" << mutationFrequencyCutoff << "\n";
    s << "time=" << time << "\n";
    s << "calculationTime=" << calculationTime << "\n";
    s << "populationSize=" << populationSize << "\n";
    return s.str();
}

void Tumor::printResults(string outfilePrefix){
	vector<int> cells = sampleCells();

	ofstream os1((outfilePrefix + ".dr").c_str() );
	ofstream os2((outfilePrefix +  ".ps").c_str() );


	if(mutationFrequencyCutoff > 0){
		driverMutationContainer->calculateMutationFrequency();
		passengerMutationContainer->calculateMutationFrequency();
	}
	for(auto c: cells) {
		os1<< driverMutationContainer->toString(c);
		os2<< passengerMutationContainer->toString(c);
	}

	os1.close();
	os2.close();

	ofstream os3((outfilePrefix +  ".prm").c_str() );
	os3 << getParameterString();
	os3.close();

	if(symmetricReplicationProbablity < 1){
		ofstream os4((outfilePrefix +  ".df").c_str() );
		for(auto c: cells) {
			os4<< ((differentiation->get(c))?"1":"0") << "\n";
		}
		os4.close();
	}

	if(infinitizingCarryingCapacityProbability > 0){
		ofstream os5((outfilePrefix +  ".ic").c_str() );
		for(auto c: cells) {
			os5<< infiniteCarryingCapacity->get(c) << "\n";
		}
		os5.close();
	}


}


void Tumor::simulateWhilePrinting(string outfilePrefix, double populationSizeChangeCutoff){
	clock_t start = clock();
	int d = 100;
	int tmp =  populationSize;
	ofstream os5((outfilePrefix +  ".gc").c_str() );
	if (SIG_ERR == signal(SIGINT, sigcatch)) {
		printf("failed to set signal handler!");
		exit(1);
	}

	int unupdated = 0;
	int unupdatedCutoff = 100;

	int i = 0;
	printResults(outfilePrefix + "." + to_string(i));
	int prePoplationSize = populationSize;
	 for(time = 1; time <= maxTime; time++){
		 if(update()){
		 	unupdated = 0;
		 }else{
		 	unupdated++;
		 }
		 if(fabs(time/d - (double)time/d) < LDBL_EPSILON){
			 cout << "time=" << time <<"\t" << "populationSize=" <<  populationSize << endl;
			 os5 << time <<"\t" <<  populationSize << endl;
			 tmp =  populationSize;

		}
		if((double)(populationSize)/(double)prePoplationSize >= populationSizeChangeCutoff |(double)prePoplationSize/(double)(populationSize) >= populationSizeChangeCutoff){
				i++;
				printResults(outfilePrefix + "." + to_string(i));
				os5 << time <<"\t" <<  populationSize << endl;
				prePoplationSize = prePoplationSize * populationSizeChangeCutoff;

		}
		if(populationSize == 0 | populationSize >= maxPopulationSize){
				i++;
				printResults(outfilePrefix + "." + to_string(i));
			 cout << "time=" << time <<"\t" << "populationSize=" <<  populationSize << endl;
			 clock_t end = clock();
			 calculationTime = (double)(end - start) / CLOCKS_PER_SEC;
			 break;
		}
		if (s == 1){
			clock_t end = clock();
			calculationTime = (double)(end - start) / CLOCKS_PER_SEC;
			break;
		}
		if((populationSize == carryingCapacity) &  (unupdated == unupdatedCutoff)){
			clock_t end = clock();
			calculationTime = (double)(end - start) / CLOCKS_PER_SEC;
			break;
		}
	}
}


// John D. Cook, http://stackoverflow.com/a/311716/15485
void sampleWithoutReplacement
(
    int populationSize,    // size of set sampling from
    int sampleSize,        // size of each sample
    vector<int> & samples  // output, zero-offset indicies to selected items
)
{
    // Use Knuth's variable names
    int& n = sampleSize;
    int& N = populationSize;

    int t = 0; // total input records dealt with
    int m = 0; // number of items selected so far
    double u;

    while (m < n)
    {
        //u = GetUniform(); // call a uniform(0,1) random number generator
        u = doubleRand();
        if ( (N - t)*u >= n - m )
        {
            t++;
        }
        else
        {
            samples[m] = t;
            t++; m++;
        }
    }
}

vector<int> Tumor::sampleCells(){
	if(tumorSampleSize >= populationSize){
		vector<int> tmp(populationSize);
		for(int i = 0; i < populationSize; i++){
			tmp[i] = i;
		}
		return tmp;
	}else{
		vector<int> tmp(tumorSampleSize);
		sampleWithoutReplacement(populationSize, tumorSampleSize, tmp);
		return tmp;
	}
}
