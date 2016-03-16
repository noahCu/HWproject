#include<cstdio>
#include<iostream>
#include"lib.h"

int main() {
	Map map;
	inputMap("input.csv", map);
	
	vector<Map> SCC;
	Map SCCmap;
	setSCC( map, SCC, SCCmap);
	divideByDominator( SCC, SCCmap);
	vector<int> path = findPath( SCC, SCCmap);
	
	outputRes(path, "outputfile.csv");
	return 0;
}
