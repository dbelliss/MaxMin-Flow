
//
//  router.cpp
//  p5
//
//  Created by Davey Jay Belliss and Vincent Yang on 5/26/15.
//  Copyright (c) 2015 Davey Jay Belliss and Vincent Yang. All rights reserved.
//

#include <iostream>
#include "router.h"
#include "RouterRunner.h"
#include <string.h>
using namespace std;

Router::Router(CityInfo *&infos, int numCities): num(numCities)
{
	info = infos;
	infos = NULL;
	//info = new CityInfo[num];
	//memcpy(info, infos, numCities * sizeof(CityInfo));
} // Router()

int Router::setTransfers(Transfer **transfers)
{
	int q1[num];
	//int q2[num]; //memset?
	register int h1 = 0;
	register int t1 = 0;
	//int h2 = 0;
	//int t2 = 0;
	register int transferNum = 0;
	int q2[num];
	register int h2 = 0;
	register int t2 = 0;
	for (int i = 0; i < num; i++)
	{
		if (info[i].production > info[i].usage)
			q1[h1++] = i;
		else
		{
			q2[h2++] = i;
		}
	}//add net positive cities to queue


	CityInfo* start = &info[1]; // city to start from
	CityInfo* end = start;

	register int startCityNum = 0;
	register int endCityNum = 0;
	register int* cityNumP = &endCityNum;
	register int amt = 0;
	register int forwardFlow = 0; //adjIndex
	register int from = 0;
	register int backflow = 0;
	register int amt2 = 0;
	register int dest = 0;
	register int net = 0;
	int marker = 0;

	marker = 2; //use forward

	while (h1 != t1 && marker == 2)
	{
		int netPos[25000];
		memset(netPos, 0xFF, 4*25000);
		startCityNum = q1[t1++];
		start = &info[startCityNum];

		int q2[num];
		bool isNotKnown[25000];
		memset(isNotKnown, 0, 25000);
		register int h2p = 0;
		register int t2p = 0;
		int *h2 = &h2p;
		int* t2 = &t2p;
		while (start->production != start->usage)
		{



			end = findNeg(start, netPos, end, startCityNum, cityNumP, q2, isNotKnown, h2, t2 );
			start = &info[startCityNum];


			if ( (start->production - start->usage)  > (end->usage - end->production))
			{
				amt = end->usage - end->production;
				end->production = end->production + amt;
				start->production = start->production - amt;
				if (start->production > start->usage)
					q1[h1++] = startCityNum;
				// CityQ.enqueue(startCityNum);
			}// end node net becomes 0

			else
			{
				amt = start->production - start -> usage;
				end->production = end->production + amt;
				start->production = start->production - amt;
			}//endnode net != 0


			forwardFlow = 0; //adjIndex
			from = 0;
			backflow = 0;
			amt2 = 0;
			dest = endCityNum;
			net = 0;
			while (&info[dest] != start && netPos[dest] != -1)
			{

				from = netPos[dest];

				for (int i = 0; i < 8; i++)
					if (info[from].adjList[i] == dest)
					{
						forwardFlow = i;
						break;
					}

				for (int j = 0; j < 8; j++)
					if (info[dest].adjList[j] == from)
					{
						backflow = j;
						break;
					}

				amt2 = transfers[dest][backflow].amount;

				if (amt2 != 0)
				{

					if (amt > amt2) // new flow is greater than back flow
					{
						net = amt - amt2;
						transfers[dest][backflow].amount = 0;
						transfers[from][forwardFlow].amount = net;
						transfers[from][forwardFlow].destCity = dest;
						transferNum -= amt2;
						transferNum +=net;
					}

					else  // amt < amt2
					{
						net = amt2 - amt;
						transfers[dest][backflow].amount = net;
						transferNum -=amt2;
						transferNum += net;

					}
				}

				else
				{
					transfers[from][forwardFlow].destCity = dest;
					transfers[from][forwardFlow].amount += amt;
					transferNum += amt;

				}

				dest = from;
			}
		}// transfers energy startfrom from the end


	}

	endCityNum = 0;
	startCityNum = 0;
	register int *startCityNumP = &startCityNum;
	amt = 0;
	while (t2 != t2 && marker == 2)
	{
		int netNeg[25000];
		memset(netNeg, 0xFF, 4*25000);
		endCityNum = q2[t2++];
		end = &info[endCityNum];
		start = findPos(start, netNeg, end, endCityNum, startCityNumP);
		end = &info[endCityNum];

		if ((start->production - start->usage) > (end->usage - end->production))
		{
			amt = end->usage - end->production;
			end->production = end->production + amt;
			start->production = start->production - amt;

		}
		else
		{
			amt = start->production - start -> usage;
			end->production = end->production + amt;
			start->production = start->production - amt;
			if (end->production < end->usage)
				q2[h2++] = endCityNum;
		}
		register int spot = 0;
		register int from = 0;
		from = startCityNum;
		register int dest = netNeg[from];
		while (&info[from] != end)
		{
			for (register int i = 0; i < 8; i++)
			{
				if (info[from].adjList[i] == dest)
				{
					spot = i;
					break;

				}
			}
			transfers[from][spot].destCity = dest;
			transfers[from][spot].amount += amt;
			transferNum += amt;
			from = dest;
			dest = netNeg[dest];
		}
	}
	return transferNum;  // should be set to total of all transfers.
}  // setTransfers

CityInfo* Router::findPos(CityInfo* start, int* netNeg, CityInfo* end,
		int endCityNum, int* startCityNum)
{

	CityInfo* temp;
	int q2[num];
	register int h2 = 0;
	register int t2 = 0;
	register int i = 0;
	bool* isNotKnown;
	isNotKnown[endCityNum] = 1;
	*startCityNum = 0;
	while(1)
	{
		for (i = 0; i < end->adjCount; i++)
		{
			*startCityNum = end->adjList[i];
			if (!(isNotKnown[*startCityNum]))
			{
				temp = &info[*startCityNum];
				isNotKnown[end->adjList[i]] = true;
				if (temp->usage < temp->production)
				{
					q2[h2++] = *startCityNum;

					netNeg[*startCityNum] = endCityNum;
					start = &info[*startCityNum];
					return start;
				}
				else
				{
					q2[h2++] = *startCityNum;
					netNeg[*startCityNum] = endCityNum;

				}
			}
		}
		endCityNum = q2[t2++];
		end = &info[endCityNum];
	}
}

CityInfo* Router::findNeg( CityInfo* start, int* netPos, CityInfo* end, int startCityNum, int* endCityNum, int* q2, bool* isNotKnown, int* h2, int* t2)
{

	CityInfo* temp;

	register int i = 0;
	isNotKnown[startCityNum] = 1;
	*endCityNum = 0;
	while (1)
	{
		for ( i = 0; i < start->adjCount; i++)
		{
			*endCityNum = start->adjList[i];
			if (!(isNotKnown[*endCityNum]))
			{
				temp = &info[*endCityNum];
				isNotKnown[start->adjList[i]] = true;
				if (temp->usage > temp->production)
				{
					isNotKnown[start->adjList[i]] = true;
					q2[(*h2)++] = *endCityNum;
					netPos[*endCityNum] = startCityNum;
					end = &info[*endCityNum];
					return end;
				}//if negative is found

				else
				{
					q2[(*h2)++] = *endCityNum;
					//CityQ2.enqueue(*endCityNum);
					netPos[*endCityNum] = startCityNum;
				}// not negative. Add to queue, keep searching
			}
		}
		startCityNum = q2[(*t2)++];
		//startCityNum = CityQ2.dequeue();
		start = &info[startCityNum];// changes current node to next in queue
	}
}// finds first negative

