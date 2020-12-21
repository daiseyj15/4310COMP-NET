#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <list>
#include <unordered_map>
#include <cstring>
using namespace std;


class graph
{
      private:
                //node               <des,          cost,nexthop>
      unordered_map<int,unordered_map<int, list<pair<int,int>>>> topo;
      int rounds=0;
      int lastNodeConv;
      int numOfDVpacket=0;

     public:
        graph();
        void addEdge(int x, int y,int wt);
        void printTopology();
        void exchangeInfoTilConv();
        bool isConverged();
        //show the path taken
        void routeApacket(int from,int to);
        void getRounds();
        void getLastNode2Conv();
        void getNumOfDVpackets();
        void addNodetoRT(int node,int desti,int cost,int nexxh);
        ~graph();


};

#endif // GRAPH_H
