#ifndef STATUSCONTAINER_H_
#define STATUSCONTAINER_H_

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

class StatusContainer{
	public:
		StatusContainer(int initialPopulationSize, int maxPopulationSize);
		virtual ~StatusContainer(){delete container;};

		void kill(int c);
		void replicate(int c);
		void set(int c, bool b){container[c]=b;};
		bool get(int c){return container[c];};
		int size(){return populationSize;};
	private:
		int initialPopulationSize;
		int maxPopulationSize;
		int populationSize;
		bool *container;

};

class StatusContainer2{
	public:
		StatusContainer2(int initialPopulationSize, int maxPopulationSize);
		virtual ~StatusContainer2(){delete container;};

		void kill(int c);
		void replicate(int c);
		void set(int c, bool b);
		int get(int c){return container[c];};
		int size(){return populationSize;};
	private:
		int initialPopulationSize;
		int maxPopulationSize;
		int populationSize;
		int *container;
		int counter;

};


#endif /*STATUSCONTAINER_H_*/
