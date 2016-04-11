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
	
	reEdge.criPathold(omap.t[0], omap.s[0], omap, res);
	
	if(res.empty()){
		clock_t ed = clock();
		YenPath bpath;
		res.push_back(bpath);
		reEdge.criPath(omap.t[0], omap.s[0], omap, res[0], 8.5 - ((ed - be) / CLOCKS_PER_SEC));
		if(res[0].node.empty())betterres.edge.clear();
		else{
			ed = clock();
			betterPath(omap.t[0], omap.s[0], reEdge, omap, res, betterres, 9 - ((ed - be) / CLOCKS_PER_SEC));
		}
	}else{
		clock_t ed = clock();
		betterPath(omap.t[0], omap.s[0], reEdge, omap, res, betterres, 8.5 - ((ed - be) / CLOCKS_PER_SEC));
	}
	
	std::vector<int> path;
	for (int i = betterres.edge.size() - 1; i >= 0; i--)
		if(betterres.edge[i] != -1)path.push_back(betterres.edge[i]);
	std::string out = argv[3];
	outputRes(path, out);
	
	return 0;
}
