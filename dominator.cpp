#include <cstdio>
#include <vector>
#include <algorithm>
#include <iostream>
#include "lib.h"

using namespace std;

bool Map::traverse(int x, bool vis[]){ // x,t: 1~N
	if(x == t)return true;
	if(vis[x] == true)return false;
	vis[x] = true;
	Vertex cur = v[x];
	for(int i = 1; i <= cur.e.size(); i++){
		bool tmp = false;
		if(vis[cur[i].to] == false)tmp = traverse(cur[i].to, vis);
		if(tmp == true)return true;
	}
	return false;
}

void Map::findDominator(){
	for(int i = 1; i <= N; i++){
		if(i == s)v[s].isDomin = true;
		else if(i == t)v[t].isDomin = true;
		else{
			bool vis[N + 5];
			memset(vis, 0, sizeof(0));
			vis[i] = true;
			if(!traverse(s, vis))v[i].isDomin = true;
			else v[i].isDomin = false;
		}
	}
}












