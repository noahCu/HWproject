#include<cstdio>
#include<iostream>
#include"lib.h"

int main() {
	Map map;
	map<int> isCritical;
	inputMap("input.csv", "criticalinput.csv",  map, isCritical);
	
	vector<Map> SCC;
	Map SCCmap;
	setSCC( map, SCC, SCCmap);
	vector<int> path = findPath( SCC, SCCmap, isCritical);
	
	outputRes(path, "outputfile.csv");
	return 0;
}
