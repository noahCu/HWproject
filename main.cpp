#include<cstdio>
#include<iostream>
#include"lib.h"

int main() {
	Map map;
	std::map<int, bool> isCritical;
	inputMap("input.csv", "criticalinput.csv",  map, isCritical);
	
	std::vector<Map> SCC;
	Map SCCmap;
	setSCC( map, SCC, SCCmap);
	std::vector<int> path = findPath( SCC, SCCmap, isCritical);
	
	outputRes(path, "outputfile.csv");
	return 0;
}
