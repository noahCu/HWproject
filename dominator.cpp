#include <cstdio>
#include <vector>
#include <algorithm>
#include <iostream>
#include "lib.h"

using namespace std;

bool Map::tran(int x){ // x,t: 1~N
	if(x == t)return true;
	if(vis[x] == true)return false;
	vis[x] = true;
	Vertex cur = v[x];
	for(int i = 1; i <= cur.e.size(); i++){
		bool tmp = false;
		if(vis[cur[i].to] == false)tmp = tran(cur[i].to, t);
		if(tmp == true)return true;
	}
	return false;
}

void Map::findDominator(){
	for(int i = 1; i <= N; i++){
		if(i == s)domin.push_back(s);
		else if(i == t)domin.push_back(t);
		else{
			memset(vis, 0, sizeof(0));
			vis[i] = true;
			if(!tran(s))domin.push_back(i);
		}
	}
}












