#ifndef ROUTER_H
#define	ROUTER_H

#include "RouterRunner.h"

using namespace std;


class Router {
private:
    
    CityInfo *info;
    int num;
public:
    Router(CityInfo *&infos, int numCities);
    int setTransfers(Transfer **transfers);
    CityInfo* findNeg( CityInfo* start, int* netPos, CityInfo* end, int startCityNum, int* endCityNum, int* q2, bool* isNotKnown, int* h2, int* t2);
    CityInfo* findPos(CityInfo* start, int* netNeg, CityInfo* end, int endCityNum,
                      int* startCityNum);
}; // class Router

#endif	// ROUTER_H

