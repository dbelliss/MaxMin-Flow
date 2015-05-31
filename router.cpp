
//
//  router.cpp
//  p5
//
//  Created by Davey Jay Belliss on 5/26/15.
//  Copyright (c) 2015 Davey Jay Belliss. All rights reserved.
//

#include <iostream>
#include "router.h"
#include "RouterRunner.h"
#include <string.h>
using namespace std;

Router::Router(CityInfo *infos, int numCities): num(numCities)
{
    info = new CityInfo[num];
    memcpy(info, infos, numCities * sizeof(CityInfo));
} // Router()

int Router::setTransfers(Transfer **transfers)
{
    
    Queue<int> CityQ(num);
    Queue<int> CityQ2(num);
    int transferNum = 0;
    
    for (int i = 0; i < num; i++)
    {
        if (info[i].production > info[i].usage)
            CityQ.enqueue(i);
    }//add net positive cities to queue
    
   
    CityInfo* start = &info[1]; // city to start from
    CityInfo* end = start;

    int cityNum1 = 0;
    int cityNum2 = 0;
    int* cityNumP = &cityNum2;
    int amt = 0;
      while (!CityQ.isEmpty())
      {
          int netPos[25000] = {-1};
         
          
          cityNum1 = (CityQ.dequeue());
          start = &info[cityNum1];
          end = findNeg(start, netPos, end, cityNum1, cityNumP );
          start = &info[cityNum1];
          
          
          if (start->production > (end->usage - end->production))
          {
              amt = end->usage - end->production;
              end->production = end->production + amt;
              start->production = start->production - amt;
              if (start->production > start->usage)
                  CityQ.enqueue(cityNum1);
          }// end node net becomes 0
          
          else
          {
              amt = start->production - start -> usage;
              end->production = end->production + amt;
              start->production = start->production - amt;
          }//endnode net != 0
          
          int spot = 0; //adjIndex
          int from = 0;
          int dest = cityNum2;
          while (&info[dest] != start && netPos[dest] != -1)
          {
             
              from = netPos[dest];
              
              for (int i = 0; i < 8; i++)
                  if (info[from].adjList[i] == dest)
                  {
                    spot = i;
                      break;
                  }
              transfers[from][spot].destCity = dest;
              transfers[from][spot].amount += amt;
              transferNum += amt;
              dest = from;
          }// transfers energy startfrom from the end
          
      }//loops while there are still net positive nodes
    

    return transferNum;  // should be set to total of all transfers.
}  // setTransfers



CityInfo* Router::findNeg( CityInfo* start, int* netPos, CityInfo* end, int cityNum1, int* cityNum2)
{
    Queue<int> CityQ2(num); 
    CityInfo* temp;
    bool isNotKnown[25000] = {0};
    int i = 0;
    isNotKnown[cityNum1] = 1;
    *cityNum2 = 0;
    while (1)
    {
        for ( i = 0; i < start->adjCount; i++)
        {
             *cityNum2 = start->adjList[i];
            if (!(isNotKnown[*cityNum2]))
            {
                temp = &info[*cityNum2];
                isNotKnown[start->adjList[i]] = true;
                if (temp->usage > temp->production)
                {
                    netPos[*cityNum2] = cityNum1;
                    end = &info[*cityNum2];
                    return end;
                }//if negative is found
            
                else
                {
                    CityQ2.enqueue(*cityNum2);
                    netPos[*cityNum2] = cityNum1;
                }// not negative. Add to queue, keep searching
            }
        }
        
        cityNum1 = CityQ2.dequeue();
        start = &info[cityNum1];// changes current node to next in queue
    }
}// finds first negative
