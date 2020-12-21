#include <iostream>
#include "graph.h"
#include <string>
#include <fstream>
using namespace std;

int main()
{
    int p1,p2,cost;
    //from topology.txt node from 1st data to 2nd data and the cost is the 3rd data given
    //routing table has <destination, cost, nexthop>
    graph topology;
    ifstream thefile;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    thefile.open("topology1.txt");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(thefile.fail())
    {
        cout << "Error opening topology (topology?.txt)" <<endl;
		exit(1);
     }

    int nore,nore2; //had to add this because last data went in twice
	while(thefile)
    {
        thefile>>p1>>p2>>cost;
        if(nore==p1&&nore2==p2)
            break;
        topology.addEdge(p1,p2,cost);
        nore=p1;
        nore2=p2;
	}

	thefile.close();
    cout<<"The Topology Initialized with Starting off Routing Tables"<<endl;
    cout<<"NODE  {ROUTING TABLE}"<<endl<<endl;
	topology.printTopology();
    topology.exchangeInfoTilConv();

    cout<<endl<<endl<<"The Routing Tables after Convergence"<<endl;
    cout<<"NODE {ROUTING TABLE}"<<endl<<endl;
    topology.printTopology();



    topology.getRounds();
    topology.getLastNode2Conv();
    topology.getNumOfDVpackets();

/////////////1st# from 2nd# to///////////////////////////////////////////////////////////////////////////////////////////////
   topology.routeApacket(0,1);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    return 0;
}
