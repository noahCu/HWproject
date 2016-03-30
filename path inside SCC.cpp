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
		dis[i].preEdge = -1;
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
					dis[to].preEdge = v[tmp.x].e[i].edgeID;
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
	edgepath.clear();
	std::vector<int> path; // store vertex index
	path.clear();
	std::vector<SPath> dis;
	bool valid[v.size() + 5];
	for(int i = 0; i < v,size(); i++)valid[i] = true;
	
	reEdge.shortestPath(end, dis, valid);
	if(dis[start].val == INF)return;
	
	YenPath init;
	init.node.clear();   init.edge.clear();
	int tmp = start;
	init.edge.push_back(-1);
	while(tmp != end)
		init.node.push_back(tmp), init.edge.push_back(dis[tmp].preEdge), 
		tmp = dis[tmp].pre;
	init.node.push_back(end);
	init.totalLen = dis[start].val;   init.Pre = start;   init.X = init.node[1];
	init.h = dis[start].val;
	
	priority_queue<YenPath, vector<YenPath>, Yencmp> q;
	q.clear();   q.push(init);
	bool flag = true; // whether is operating the shortest path
	bool ansflag = false; // whether path is found
	YenPath ans;
	while(!q.empty()){
		YenPath tmp = q.top();   q.pop();
		if(this.checkValid(start, end, tmp)){
			ans = tmp;   ansflag = true;   break;
		}
		
		for(int i = 0; i < v.size(); i++)valid[i] = true;
		for(int i = 0; i < tmp.node.size(); i++)
			if(tmp.node[i] != start && tmp.node[i] != end)valid[tmp.node[i]] = false;
		// delete invalid nodes
		
		reEdge.shortestPath(end, dis, valid); // get shortest path from end
		
		tmp.len = tmp.totalLen;   tmp.x = tmp.node.size() - 1;   tmp.pre = tmp.x - 1;
		// set init value
		while(tmp.node[tmp.x] != tmp.Pre){
			int x = tmp.node[tmp.x];
			int pre = tmp.node[tmp.pre];
			if(valid[x] == false){
				valid[x] = true;
				reEdge.updateSPath(x, end, dis, valid);
			}// update shortest path after adding new node
			for(int i = 0; i < v[pre].e.size(); i++)
				if(v[pre].e[i].edgeID == tmp.edge[tmp.x]){
					tmp.len -= v[pre].e[i].val;   break;
				}// update tmp.len : from start to pre
			priority_queue<YenNewPath, vector<YenNewPath>, YenNewPathcmp> newq;// queue of new path
			newq.clear();
			for(int i = 0; i < v[pre].e.size(); i++){
				YenNewPath newstate;
				newstate.edgeID = v[pre].e[i].edgeID;
				newstate.x = v[pre].e[i].to;
				newstate.h = tmp.len + v[pre].e[i].val + dis[v[pre].e[i].to].val;
				// h fuction may need change
				newq.push(newstate);
			}// find all the deviation states
			
			
			
		}
		
	}
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
