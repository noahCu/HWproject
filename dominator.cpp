#include <cstdio>
#include <vector>
#include <algorithm>
#include <iostream>
#include "lib.h"

using namespace std;

bool Map::traverse(int x, int end, bool vis[], std::vector<int> & path){ // x,t: 1~N
	if(x == end){
		path.push_back(end);
		return true;
	}
	if(vis[x] == true)return false;
	vis[x] = true;
	Vertex cur = v[x];
	for(int i = 0; i < cur.e.size(); i++){
		bool tmp = false;
		if(vis[cur[i].to] == false)tmp = traverse(cur[i].to, end, vis, path);
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
			if(!traverse(start, end, vis, path))v[i].isDomin = true;
			else v[i].isDomin = false;
		}
	}
}












