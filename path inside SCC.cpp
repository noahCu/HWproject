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
		SPath tmp = *q.top();   q.pop();
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

void Map::updateSPath(int newnode, int start, std::vector<SPath> & dis, bool valid[], const Map & reEdge){
	for(int i = 0; i < reEdge[newnode].e.size(); i++){
		int to = reEdge[newnode].e[i].to;
		int val = reEdge[newnode].e[i].val;
		if(valid[to] && dis[to].val != INF && dis[newnode].val > dis[to].val + val){
			dis[newnode].val = dis[to].val + val;
			dis[newnode].pre = to;
			dis[newnode].preEdge = reEdge[newnode].e[i].edgeID;
		}
	}
	bool vis[v.size() + 5];
	for(int i = 0; i < v.size(); i++)vis[i] = false;
	queue<SPath*> q;   q.clear();
	q.push(&dis[newnode]);
	vis[newnode] = true;
	while(!q.empty()){
		SPath tmp = *q.top();   q.pop();
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

bool Map::checkValid(int start, int end, const YenPath & path){
	if(path.node[0] != start || path.node[path.node.size() - 1] != end)return false;
	int numOfCri = 0;
	for(int i = 0; i < v.size(); i++)if(v[i].isCritical)numOfCri++;
	int ansCri = 0;
	for(int i = 0; i < path.node.size(); i++)if(v[path.node[i]].isCritical)ansCri++;
	if(ansCri == numOfCri)return true;
	return false;
}

void makeNewstate(YenPath & newstate, const YenNewPath & curP, const YenPath & tmp, const std::vector<SPath> & dis){
	newstate.node.clear();
	for(int i = 0; i <= tmp.pre; i++)newstate.node.push_pack(tmp.node[i]);
	newstate.edge.clear();
	for(int i = 0; i <= tmp.pre; i++)newstate.edge.push_pack(tmp.edge[i]);
	int curver = curP.x;
	newstate.edge.push_back(curP.edgeID);
	while(curver != end){
		newstate.node.push_back(curver);
		newstate.edge.push_back(dis[curver].preEdge);
		curver = dis[curver].pre;
	}
	newstate.node.push_back(end);
	newstate.Pre = tmp.node[tmp.pre];   newstate.X = curP.x;   newstate.h = curP.h;
	newstate.totalLen = tmp.len + curP.edgeLen + dis[curP.x].val;
}

void Map::criPath(int start, int end, Map & reEdge, YenPath & edgepath){
	std::vector<SPath> dis;
	bool valid[v.size() + 5];
	for(int i = 0; i < v.size(); i++)valid[i] = true;
	
	reEdge.shortestPath(end, dis, valid);
	if(dis[start].val == INF){
		edgepath.node.clear();
		edgepath.edge.clear();
		return;
	}
	
	YenPath init;
	init.node.clear();   init.edge.clear();
	int tmpver = start;
	init.edge.push_back(-1);
	while(tmpver != end)
		init.node.push_back(tmpver), init.edge.push_back(dis[tmpver].preEdge), 
		tmpver = dis[tmpver].pre;
	init.node.push_back(end);
	init.totalLen = dis[start].val;   init.Pre = start;   init.X = init.node[1];
	init.h = dis[start].val;
	
	priority_queue<YenPath, vector<YenPath>, Yencmp> q;
	q.clear();   q.push(init);
	bool flag = true; // whether is operating the shortest path
	bool ansflag = false; // whether path is found
	while(!q.empty()){
		YenPath tmp = q.top();   q.pop();
		if(this.checkValid(start, end, tmp)){
			edgepath = tmp;   ansflag = true;   break;
		}
		
		for(int i = 0; i < v.size(); i++)valid[i] = true;
		for(int i = 0; i < tmp.node.size(); i++)
			if(tmp.node[i] != end)valid[tmp.node[i]] = false;
		// delete invalid nodes
		
		reEdge.shortestPath(end, dis, valid); // get shortest path from end
		
		tmp.len = tmp.totalLen;   tmp.x = tmp.node.size() - 1;   tmp.pre = tmp.x - 1;
		// set init value
		while(tmp.node[tmp.x] != tmp.Pre){
			int x = tmp.node[tmp.x];
			int pre = tmp.node[tmp.pre];
			if(valid[x] == false){
				valid[x] = true;
				reEdge.updateSPath(x, end, dis, valid, this);
			}// update shortest path after adding new node
			for(int i = 0; i < v[pre].e.size(); i++)
				if(v[pre].e[i].edgeID == tmp.edge[tmp.x]){
					tmp.len -= v[pre].e[i].val;   break;
				}// update tmp.len : from start to pre
			priority_queue<YenNewPath, vector<YenNewPath>, YenNewPathcmp> newq;// queue of new path
			newq.clear();
			for(int i = 0; i < v[pre].e.size(); i++){
				if(valid[v[pre].e[i].to] == false)continue;
				if(v[pre].e[i].edgeID == tmp.edge[tmp.x])continue;
				YenNewPath newstate;
				newstate.edgeID = v[pre].e[i].edgeID;
				newstate.x = v[pre].e[i].to;
				newstate.h = tmp.len + v[pre].e[i].val + dis[v[pre].e[i].to].val;
				// h fuction may need change
				newstate.edgeLen = v[pre].e[i].val;
				newq.push(newstate);
			}// find all the deviation states
			while(!newq.empty() && ((flag == true && newq.top().h < tmp.h) || 
				(flag == false && newq.top().h <= tmp.h)))newq.pop(); 
			// ignore repeat states
			if(!newq.empty()){
				YenNewPath curP = newq.top();
				newq.pop();
				YenPath newstate;
				makeNewstate(newstate, curP, tmp, dis);
				q.push(newstate);
				// add the shortest new path to q
				int lasth = curP.h;
				while(!newq.empty() && newq.top().h == lasth){
					curP = newq.top();   newq.pop();
					makeNewstate(newstate, curP, tmp, dis);
					q.push(newstate);
				}// add other new path with the same h val to q
			}
			tmp.pre--;   tmp.x--;
		}
		flag = false;
	}
	if(ansflag == false){
		edgepath.node.clear();
		edgepath.edge.clear();
	}
	// ans
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
