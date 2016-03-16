#include"lib.h"
#include<fstream.h>
#include<map>
#include<set>
using namespace std;

void inputMap( String mapfile, String cfile, Map & omap, std::set<int> & isCritical) {
	ifstream mapRead(mapfile);
	ifstream cRead(cfile);
	std::map<int, int> idRef;

	int from, to, val, edgeID;
	char comma;
	while (!mapRead.eof()) {
		mapRead >> edgeID >> comma >> from >> comma >> to >> comma >> val;
		if (idRef.find(from) == idRef.end()) {
			idRef[from] = omap.push_back();
		}
		if (idRef.find(to) == idRef.end()) {
			idRef[to] = omap.push_back();
		}
		from = idRef[from];
		to = idRef[to];
		omap[from].push_back(Edge(from, to, val, edgeID);
	}

	int s, t;
	char ch;
	cRead >> s >> ch >> t >> ch;
	omap.s = idRef[s];
	omap.t = idRef[t];

	int cri;
	while (!cRead.eof()) {
		cRead >> cri >> ch;
		isCritical[ idRef[cri] ] = 1;
	}


	mapRead.close();
	cRead.close();
}

void outputRes( const vector<int> & path, const String & outputfile) {
	ofstream resWrite(outputfile);
	if (path.empty()) {
		resWrite << "NA";
	} else {
		for (auto itr = path.begin(); itr != path.end(); itr++) {
			if (itr != path.begin()) resWrite << '|';
			resWrite << *itr;
		}
	}
}

