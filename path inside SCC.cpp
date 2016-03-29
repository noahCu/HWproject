#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <queue>
#include "lib.h"

using namespace std;

void Map::reverse(Map & reEdge){
	reEdge.v.clear();
	for(int i = 0; i < v.size(); i++){
		reEdge.push_back(v[i].ID);
		reEdge[i].isCritical = v[i].isCritical;
		reEdge[i].isDomin = v[i].isDomin;
		reEdge[i].e.clear();
	}
	reEdge.s.clear();
	reEdge.t.clear();
	for(int j = 0; j < s.size(); j++)reEdge.s.push_back(s[j]);
	for(int j = 0; j < t.size(); j++)reEdge.t.push_back(t[j]);
	for(int i = 0; i < v.size(); i++){
		for(int j = 0; j < v[i].e.size(); j++){
			int from = v[i].e[j].to;
			int to = i;
			int val = v[i].e[j].val;
			int edgeID = v[i].e[j].edgeID;
			reEdge[from].push_back( Edge(from, to, val, edgeID));
		}
	}
}

void Map::shortestPath(int start, std::vector<SPath> & dis, bool valid[]){
	queue<SPath*> q;
	q.clear();
	dis.clear();
	bool vis[v.size() + 5];
	for(int i = 0; i < v.size(); i++){
		dis.push_back();
		dis[i].pre = -1;
		dis[i].val = INF;
		dis[i].x = i;
		vis[i] = false;
	}
	vis[start] = true;
	dis[start].pre = start;
	dis[start].val = 0;
	q.push(&dis[start]);
	while(!q.empty()){
		SPath tmp = *q.top();
		q.pop();
		for(int i = 0; i < v[tmp.x].e.size(); i++){
			int to = v[tmp.x].e[i].to;
			int val = v[tmp.x].e[i].val;
			if(valid[to] == true){
				if(dis[to].val > dis[tmp.x].val + val){
					dis[to].val = dis[tmp.x].val + val;
					dis[to].pre = tmp.x;
					if(vis[to] == false){
						vis[to] = true;
						q.push(&dis[to]);
					}
				}
			}
		}
		vis[tmp.x] = false;
	}
}

void Map::criPath(int start, int end, Map & reEdge, std::vector<int> & edgepath){
	std::vector<int> path; // store vertex index
	path.clear();
	
}

void Map::pathInSCC(int start, int end, std::vector<int> & path){// path store ID of edge
	std::vector< Map > D;
	divideByDominator(start, end, D);
	path.clear();
	for(int i = 0; i < D.size(); i++){
		Map reEdge;
		D[i].reverse(reEdge);
		D[i].criPath(D[i].s[0], D[i].t[0], reEdge, path);
	}
}
