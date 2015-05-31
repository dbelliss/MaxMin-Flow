#ifndef ROUTER_H
#define	ROUTER_H
#include <vector>
#include "RouterRunner.h"
#include "QueueAr.cpp"
using namespace std;


class Router {
private:

    CityInfo *info;
    int num;
public:
    Router(CityInfo *infos, int numCities);
    int setTransfers(Transfer **transfers);
    CityInfo* findNeg( CityInfo* start, int* netPos, CityInfo* end, int cityNum1, int* cityNum2);
}; // class Router

#endif	// ROUTER_H

