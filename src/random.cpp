#include "random.h"


std::random_device rd;
std::mt19937 mt;
std::uniform_real_distribution<double> dist;

void initRand()
{
     mt = std::mt19937(rd());
     dist = std::uniform_real_distribution<double>(0.0, 1.0) ;
}

double doubleRand()
{    
     return dist(mt);
}   


bool boolRand()
{
    if(dist(mt)<0.5){
          return true;
     }else{
          return false;
     }
}
