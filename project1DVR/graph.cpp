#include "graph.h"
#include <iostream>
#include <list>
#include <unordered_map>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;

graph::graph()
{
    //ctor
}

void graph::addEdge(int x, int y, int wt)
{
    // node   <des,cost,nexthop>
    topo[x][y].push_back(make_pair(wt,y));
    topo[y][x].push_back(make_pair(wt,x));
}

void graph::printTopology()
{
    cout<<endl;
    for(auto p: topo)
    {
        int srcnode = p.first;
        unordered_map<int,list<pair<int,int>>> p2 = p.second;

        cout<<srcnode<<"  {";
        for(auto routingtable: p2)
        {
            //    <des,cost,nexthop>
            int dest = routingtable.first;
            list<pair<int,int>> rt = routingtable.second;

            cout<<"<"<<dest<<",";
            for(auto rout: rt)
            {
                int cost = rout.first;
                int nh = rout.second;

                cout<<cost<<","<<nh<<">  ";
            }

        }
        cout<<" }"<<endl;
    }
}

//transfering dv packets <can reach,cost>the NH would be the node sending the info
void graph::exchangeInfoTilConv()
{
    unordered_map<int,vector<int>> neighbors;
    //recording the neighbors of each node for nexthop purposes
        for(auto b:topo)
        {
            int nod = b.first;
            unordered_map<int,list<pair<int,int>>> nay = b.second;
            for(auto c: nay)
            {
                neighbors[nod].push_back(c.first);
            }
        }


    bool tORf = false;
    while(!tORf)
    {
        rounds++;
        for(auto d:topo)//map<int,map<int,list<int,int>>>
        {
           int nodeOn = d.first;
           unordered_map<int,list<pair<int,int>>> routTofnodeOn = d.second;//RT for NODE SENDING PACKETS
           vector<int> naybrs= neighbors[nodeOn]; //getting neighbors of node we are on that is sending packets

            //send a packet to each neighbor
           for(int e=0;e<naybrs.size();e++)////////////////////////////////////////////
           {
               numOfDVpacket++;
              //loop to go thru RT of the neighbor node
              unordered_map<int,list<pair<int,int>>> rtOfnay = topo[naybrs[e]];

               for(auto reciever:rtOfnay)//map<int,list<int,int>>//dont need to send to all nodes
               {
                   int nodeINrtOfREC = reciever.first;//////
                   if(nodeINrtOfREC == naybrs[e])
                   {
                        continue;
                   }
                   list<pair<int,int>> costNDnhOFrec = reciever.second;////
                   if(find(naybrs.begin(), naybrs.end(), nodeINrtOfREC) != naybrs.end())
                    {
                        /* naybrs contains nodeINrtOfREC */
                    }
                    else
                    {
                        continue;
                    }

                   for(auto sender: routTofnodeOn)//map<int,list<int,int>> //sender RT
                   {
                       int nodesINsenderRT =reciever.first;// sender.first;
                       list<pair<int,int>> cNDnh4sendRT = reciever.second;//sender.second;

                       if(nodesINsenderRT==naybrs[e])
                       {
                           continue;
                       }
                        //send (nodeOn) info to neighbors
                        //check if reciever has all nodes sender has in its RT
                        unordered_map<int,list<pair<int,int>>>::const_iterator got = rtOfnay.find(nodesINsenderRT);//changed
                        if ( got == rtOfnay.end())//
                        {   //not found
                            //adding the node to the recievers RT
                            //map<int,map<int,list<int,int>>>
                           int woow = sender.first;
                            //unordered_map<int,list<pair<int,int>>> hmmmmmmm({woow,make_pair(((sender.second).front()).first,nodeOn)});
                            //topo[naybrs[e]].insert(hmmmmmmm);
                           // topo[naybrs[e]][nodesINsenderRT].push_back(make_pair((cNDnh4sendRT.front()).first,nodeOn));
                          addNodetoRT(naybrs[e],nodesINsenderRT,(cNDnh4sendRT.front()).first,nodeOn);
                          // cout<<endl<<endl<<"  {"<<(topo[naybrs[e]][nodesINsenderRT].front()).first<<endl;
                            lastNodeConv = naybrs[e];
                        }

                        //check if it contains a lower cost if so change cost and nexthop to that node
                        //may need to check this math
                        int isCostLower = (topo[naybrs[e]][nodeOn].front()).first + (topo[nodeOn][nodesINsenderRT].front()).first;

                        if(isCostLower < (topo[naybrs[e]][nodesINsenderRT].front()).first)
                        {

                            topo[naybrs[e]][nodesINsenderRT].pop_back();
                            topo[naybrs[e]][nodesINsenderRT].push_front(make_pair(isCostLower,nodeOn));

                            lastNodeConv = naybrs[e];
                        }
                   }
               }
           }
        }

        tORf=isConverged();
    }
}

bool graph::isConverged()
{
    int x=0;
    vector<int> allNodes;
    for(auto a:topo)
    {
        allNodes.push_back(a.first);
    }

    for(auto b: topo)//<int,map>
    {
        unordered_map<int,list<pair<int,int>>> chekRT = b.second;
        x=0;
        for(auto routt: chekRT)
        {
            int destt = routt.first;

                 if(allNodes[x] == destt ||find(allNodes.begin(),allNodes.end(),destt) != allNodes.end())
                   {
                       continue;
                   }
                   else
                   {
                      return false;
                   }
             x++;
        }
     }

    for(auto q: allNodes)
    {
        for(auto r: allNodes)
        {
            if(q==r)
            {
                continue;
            }

           if((topo[q][r].front()).first != (topo[r][q].front()).first)
           {
               return false;
           }
        }
    }


    return true;
}
//show the path taken // node   <des,cost,nexthop>
void graph::routeApacket(int from, int to)
{
   vector<int> pathh;
   int x = from;

   while(x != to)
   {
       pair<int,int> nexthop = topo[x][to].front();
       pathh.push_back(nexthop.second);
       x = nexthop.second;
   }

   cout<<endl<<endl<<"The path from "<< from;
   cout<<" to "<<to<< " is:"<<endl<<from<<" -> ";

   for(auto d=0;d < pathh.size();d++)
   {
       cout<<pathh[d];
       if(d<(pathh.size()-1))
       {
          cout<<" -> ";
       }

   }
    cout<<endl;
}
void graph::getRounds()
{
    cout<<endl<<"1. How many rounds did it take each network to converge? ";
    cout<<rounds<<endl;
}
void graph::getLastNode2Conv()
{
    cout<<"2. What is the ID of the last node to converge in each network? ";
    cout<<lastNodeConv<<endl;
}
void graph::getNumOfDVpackets()
{
    cout<<"3. How many DV messages were sent in total until each network converged? ";
    cout<<numOfDVpacket<<endl;
}

void graph::addNodetoRT(int node,int desti,int cost,int nexxh)
{
    // node   <des,cost,nexthop>
    topo[node][desti].push_back(make_pair(cost,nexxh));
}
graph::~graph()
{
    //dtor
}
