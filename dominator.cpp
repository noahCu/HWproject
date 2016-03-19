#include <cstdio>
#include <vector>
#include <algorithm>
#include <iostream>
#include "lib.h"

using namespace std;

bool Map::findDominatorPath(int x, int end, bool vis[], std::vector<int> & path){ // x,t: 1~N
	if(x == end){
		path.push_back(end);
		return true;
	}
	if(vis[x] == true)return false;
	vis[x] = true;
	Vertex cur = v[x];
	for(int i = 0; i < cur.e.size(); i++){
		bool tmp = false;
		if(vis[cur[i].to] == false)tmp = findDominatorPath(cur[i].to, end, vis, path);
		if(tmp == true){
			path.push_back(x);
			return true;
		}
	}
	return false;
}

void Map::findDominator(int start, int end){
	for(int i = 0; i < N; i++){
		if(i == start)v[start].isDomin = true;
		else if(i == end)v[end].isDomin = true;
		else{
			bool vis[N + 5];
			for(int j = 0; j < N; j++)vis[j] = false;
			vis[i] = true;
			std::vector<int> path;
			path.clear();
			if(!findDominatorPath(start, end, vis, path))v[i].isDomin = true;
			else v[i].isDomin = false;
		}
	}
}

int traverse(int x, int end, int vis[], Map & newD){
	int verindex = newD.push_back(v[x].ID);
	newD[verindex].isCritical = v[x].isCritical;
	newD[verindex].e.clear();
	vis[x] = verindex;
	if(x == end)return verindex;
	for(int i = 0 ; i < v[x].e.size(); i++){
		if(vis[v[x].e[i].to] == -1){
			int tmpverindex = traverse(v[x].e[i].to, end, vis, newD);
			int from = verindex, to = tmpverindex, val = v[x].e[i].val, ID = v[x].e[i].edgeID;
			newD[verindex].push_back( Edge(from, to, val, ID));
		}else{
			int from = verindex, to = vis[v[x].e[i].to], val = v[x].e[i].val, ID = v[x].e[i].edgeID;
			newD[verindex].push_back( Edge(from, to, val, ID));
		}
	}
}

void divideByDominator(int start, int end, std::vector< Map > & D, Map & Dmap ){
	findDominator(start, end);
	bool vis[N + 5];
	for(int i = 0; i < N; i++)vis[i] = false;
	std::vector<int> path;
	path.clear();
	findDominatorPath(start, end, vis, path);
	D.clear();
	int last = -1, cur = start;
	for(int i = path.size() - 2; i >= 0; i--){
		if(v[path[i]].isDomin == true){
			last = cur;
			cur = path[i];
			D.push_back();
			int vis[N + 5];
			for(int j = 0; j < N; j++)vis[j] = -1;
			traverse(last, cur, vis, D[D.size() - 1]);
			D[D.size() - 1].s.push_back(vis[last]);
			D[D.size() - 1].t.push_back(vis[cur]);
		}
	}
}










