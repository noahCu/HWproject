#include <cstdio>
#include <vector>
#include <algorithm>
#include <iostream>
#include "lib.h"

using namespace std;

bool Map::tran(int x , int t){ // x,t: 1~N
	if(x == t)return true;
	if(vis[x] == true)return false;
	vis[x] = true;
	for(int i = 1; i <= e[x].size(); i++){
		bool tmp = false;
		if(vis[e[x][i].to] == false)tmp = tran(e[x][i].to, t);
		if(tmp == true)return true;
	}
	return false;
}

void Map::findDominator(int s, int t){ // s,t: 1~N
	for(int i = 1; i <= N; i++){
		if(i == s)domin.push_back(s);
		else if(i == t)domin.push_back(t);
		else{
			memset(vis, 0, sizeof(0));
			vis[i] = true;
			if(!tran(s, t))domin.push_back(i);
		}
	}
}












