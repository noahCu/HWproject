#include"lib.h"
#include<fstream>
using namespace std;

void inputMap( string mapfile, string cfile, Map & omap, std::map<int, bool> & isCritical) {
	ifstream mapRead(mapfile.c_str());
	ifstream cRead(cfile.c_str());
	std::map<int, int> idRef;

	int from, to, val, edgeID;
	char comma;
	while (!mapRead.eof()) {
		val = -1;
		mapRead >> edgeID >> comma >> from >> comma >> to >> comma >> val;
		if (val == -1 ) break;
		if (idRef.find(from) == idRef.end()) {
			idRef[from] = omap.push_back( from );
		}
		if (idRef.find(to) == idRef.end()) {
			idRef[to] = omap.push_back( to );
		}
		from = idRef[from];
		to = idRef[to];
		omap[from].push_back( Edge(from, to, val, edgeID ));
	}

	int s, t;
	char ch;
	cRead >> s >> ch >> t >> ch;
	omap.s.push_back( idRef[s] );
	omap.t.push_back( idRef[t] );
	omap[ idRef[s] ].isCritical = 1;
	omap[ idRef[t] ].isCritical = 1;

	int cri;
	while (!cRead.eof()) {
		cri = -1;
		cRead >> cri >> ch;
		if (cri == -1) break;
		isCritical[ idRef[cri] ] = 1;
		omap[ idRef[cri] ].isCritical = 1;
	}


	mapRead.close();
	cRead.close();
}

void outputRes( const vector<int> & path, const string & outputfile) {
	ofstream resWrite(outputfile.c_str());
	if (path.empty()) {
		resWrite << "NA";
	} else {
		for (auto itr = path.begin(); itr != path.end(); itr++) {
			if (itr != path.begin()) resWrite << '|';
			resWrite << *itr;
		}
	}
}

