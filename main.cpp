#include<cstdio>
#include<iostream>
#include<time.h>
#include"lib.h"

int main(int argc, char* argv[]) {
	Map omap;
	std::map< int, bool> isCritical;
	clock_t be = clock();
	
	std::string topo = argv[1];
	std::string demand = argv[2];
	int crinum = inputMap(topo, demand, omap, isCritical);
	
	std::vector<YenPath> res;
	res.clear();
	Map reEdge;
	
	omap.reverse(reEdge);
	YenPath betterres;
	
	omap.criPathold(omap.s[0], omap.t[0], reEdge, res);
	
	if(res.empty()){
		clock_t ed = clock();
		YenPath bpath;
		res.push_back(bpath);
		omap.criPath(omap.s[0], omap.t[0], reEdge, res[0], 8 - ((ed - be) / CLOCKS_PER_SEC));
		if(res[0].node.empty())betterres.edge.clear();
		else{
			ed = clock();
			betterPath(omap.s[0], omap.t[0], omap, reEdge, res, betterres, 9 - ((ed - be) / CLOCKS_PER_SEC));
		}
	}else{
		clock_t ed = clock();
		betterPath(omap.s[0], omap.t[0], omap, reEdge, res, betterres, 8.5 - ((ed - be) / CLOCKS_PER_SEC));
	}
	
	std::vector<int> path;
	for (auto itr = betterres.edge.begin(); itr != betterres.edge.end(); itr++)
		if(itr != betterres.edge.begin())path.push_back(*itr);
	std::string out = argv[3];
	outputRes(path, out);
	
	return 0;
}
