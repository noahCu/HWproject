#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <queue>
#include <list>
#include <set>
#include <time.h>
#include "lib.h"
#define INF 100000



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
	list<SPath*> q;   q.clear();
	vector<SPath> ().swap(dis);
	bool vis[v.size() + 5];
	
	for(int i = 0; i < v.size(); i++){
		SPath tmp;
		dis.push_back(tmp);
		dis[i].pre = -1;
		dis[i].val = INF;
		dis[i].x = i;
		dis[i].preEdge = -1;
		vis[i] = false;
	}
	
	vis[start] = true;
	dis[start].pre = start;
	dis[start].val = 0;
	q.push_back(&dis[start]);
	while(!q.empty()){
		SPath tmp = *q.front();   q.pop_front();
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
						q.push_back(&dis[to]);
					}
				}
			}
		}
		vis[tmp.x] = false;
	}
}

void Map::updateSPath(int newnode, int start, std::vector<SPath> & dis, bool valid[], Map & reEdge, std::set<YenCri, YenCricmp> & criq){
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
	list<SPath*> q;   q.clear();
	q.push_back(&dis[newnode]);
	vis[newnode] = true;
	while(!q.empty()){
		SPath tmp = *q.front();   q.pop_front();
		for(int i = 0; i < v[tmp.x].e.size(); i++){
			int to = v[tmp.x].e[i].to;
			int val = v[tmp.x].e[i].val;
			if(valid[to] == true){
				if(dis[to].val > dis[tmp.x].val + val){
					/*if(v[to].isCritical){
						YenCri tmpCri;	tmpCri.x = to;	tmpCri.lenToT = dis[to].val;
						if(criq.find(tmpCri) != criq.end())criq.erase(tmpCri);
					}*/
					dis[to].val = dis[tmp.x].val + val;
					/*if(v[to].isCritical){
						YenCri tmpCri;	tmpCri.x = to;	tmpCri.lenToT = dis[to].val;
						criq.insert(tmpCri);
					}*/
					dis[to].pre = tmp.x;
					dis[to].preEdge = v[tmp.x].e[i].edgeID;
					if(vis[to] == false){
						vis[to] = true;
						q.push_back(&dis[to]);
					}
				}
			}
		}
		vis[tmp.x] = false;
	}
}

bool Map::checkValid(int start, int end, std::vector<int> & path){
	if(path[0] != start || path[path.size() - 1] != end)return false;
	int numOfCri = 0;
	for(int i = 0; i < v.size(); i++)if(v[i].isCritical)numOfCri++;
	for(int i = 0; i < path.size(); i++)if(path[i] == end && i != path.size() - 1)return false;
	int ansCri = 0;
	for(int i = 0; i < path.size(); i++)if(v[path[i]].isCritical)ansCri++;
	if(ansCri == numOfCri)return true;
	return false;
}

void makeNewstate(int end, YenPath* newstate, Edge* edge, YenPath & tmp, std::vector<SPath> & dis, set<YenCri, YenCricmp> & criq){
	newstate -> node.clear();
	for(int i = 0; i <= tmp.pre; i++)newstate -> node.push_back(tmp.node[i]);
	newstate -> edge.clear();
	for(int i = 0; i <= tmp.pre; i++)newstate -> edge.push_back(tmp.edge[i]);
	int curver = edge -> to;
	newstate -> edge.push_back(edge -> edgeID);
	while(curver != end){
		newstate -> node.push_back(curver);
		newstate -> edge.push_back(dis[curver].preEdge);
		curver = dis[curver].pre;
	}
	newstate -> node.push_back(end);
	newstate -> Pre = tmp.node[tmp.pre];   newstate -> X = edge -> to;
	newstate -> totalLen = tmp.len + edge -> val + dis[edge -> to].val;
	int tmph = newstate -> totalLen;
	/*if(!criq.empty()){
		std::set<YenCri, YenCricmp>::iterator it = criq.begin();
		if(tmph < it -> lenToT)tmph = it -> lenToT;
	}*/
	newstate -> h = tmph;
}

struct Yenanscmp{
	bool operator()(YenPath a, YenPath b){
		return a.totalLen > b.totalLen;
	}
};

bool realcheck(int start, int end, Map & omap, YenPath & path){
	if(path.node[0] != start || path.node[path.node.size() - 1] != end)return false;
	int last = path.node[0];
	bool vis[omap.v.size() + 5];
	for(int i = 0; i < omap.v.size(); i++)vis[i] = 0;
	vis[path.node[0]] = 1;
	for(int i = 1; i < path.node.size(); i++){
		bool flag = 0;
		for(int j = 0; j < omap[last].e.size(); j++)
			if(omap[last][j].edgeID == path.edge[i] && omap[last][j].to == path.node[i]){
				flag = 1;	break;
			}
		if(flag == 0)return 0;
		last = path.node[i];
		vis[last] = 1;
	}
	for(int i = 0; i < omap.v.size(); i++){
		if(omap[i].isCritical && vis[i] == 0)return 0;
	}
	return 1;
}

void Map::criPathold(int start, int end, Map & reEdge, std::vector<YenPath> & edgepath){
	std::vector<SPath> dis;
	bool valid[v.size() + 5];
	for(int i = 0; i < v.size(); i++)valid[i] = true;
	clock_t be = clock();
	
	reEdge.shortestPath(end, dis, valid);
	if(dis[start].val == INF){
		edgepath.clear();
		return;
	}

	/*for(int i = 0; i < v.size(); i++){
		if(v[i].isCritical)valid[i] = false;else valid[i] = true;
	}
	reEdge.shortestPath(end, dis, valid);*/
	
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

	priority_queue<YenPath*, deque<YenPath*>, Yencmp> q;
	q.push(&init);
	bool flag = true; // whether is operating the shortest path
	bool ansflag = false; // whether path is found
	int numans = 0; // number of ans
	/*int occ[v.size() + 5];
	for(int i = 0; i < v.size(); i++)occ[i] = 0;
	for(int i = 0; i < init.node.size(); i++)occ[init.node[i]]++;
	int count = 0;*/
	priority_queue<YenPath, deque<YenPath>, Yenanscmp> ansq;

	while(!q.empty()){
		YenPath & tmp = *q.top(); 
		q.pop();
		if(realcheck(start, end, *this, tmp)){
			ansflag = true;
			numans++;	ansq.push(tmp);
			if(numans >= 10000)break;
		}
		
		clock_t ed = clock();
		if((ansflag == 0 && (ed - be) / CLOCKS_PER_SEC >= 3.5) || 
		(ansflag == 1 && (ed - be) / CLOCKS_PER_SEC >= 5))break;
		
		for(int i = 0; i < v.size(); i++)valid[i] = true;
		for(int i = 0; i < tmp.node.size(); i++)
			if(tmp.node[i] != end)valid[tmp.node[i]] = false;
		// delete invalid nodes
		
		reEdge.shortestPath(end, dis, valid); // get shortest path from end
		
		std::vector<SPath> constdis;
		for(int i = 0; i < dis.size(); i++){
			SPath tmpdis;
			constdis.push_back(tmpdis);
			constdis[i].pre = dis[i].pre;
			constdis[i].val = dis[i].val;
			constdis[i].x = dis[i].x;
			constdis[i].preEdge = dis[i].preEdge;
		}
		/*
		bool cutvalid[v.size() + 5], turnon = 0;
		for(int i = 0; i < v.size(); i++)cutvalid[i] = valid[i];
		
		std::vector<SPath> cutdis;
		if(count >= 10000){
			for(int i = 0; i < v.size(); i++)
				if(cutvalid[i] && occ[i] > 0.7 * count && i != start && i != end)
					cutvalid[i] = 0, turnon = 1;
			if(turnon == 1)reEdge.shortestPath(end, cutdis, cutvalid);
		}*/

		std::set<YenCri, YenCricmp> criq;
		/*for(int i = 0; i < v.size(); i++)
			if(v[i].isCritical && valid[i]){
				YenCri tmp;	tmp.x = i;	tmp.lenToT = dis[i].val;
				criq.insert(tmp);
			}*/
		
		tmp.len = tmp.totalLen;   tmp.x = tmp.node.size();   tmp.x--;   tmp.pre = tmp.x - 1;
		// set init value

		while((flag == true && tmp.node[tmp.x] != tmp.Pre) || 
		(flag == false && tmp.node[tmp.pre] != tmp.Pre)){
			int x = tmp.node[tmp.x];	
			int pre = tmp.node[tmp.pre];
			if(valid[x] == false){
				valid[x] = true;
				reEdge.updateSPath(x, end, dis, valid, *this, criq);
			}// update shortest path after adding new node
			for(int i = 0; i < v[pre].e.size(); i++)
				if(v[pre].e[i].edgeID == tmp.edge[tmp.x]){
					tmp.len -= v[pre].e[i].val;   break;
				}// update tmp.len : from start to pre
			
			/*std::set<YenCri, YenCricmp>::iterator it = criq.end();	it--;
			if(!criq.empty() && it -> lenToT >= INF){
				tmp.x--;	tmp.pre--;
				continue;
			}*/
			int maxn = 0;
			for(int i = 0; i < v.size(); i++)if(valid[i] && v[i].isCritical){
				if(maxn < dis[i].val)maxn = dis[i].val;
			}
			if(maxn >= INF){
				tmp.x--;	tmp.pre--;
				continue;
			}


			//priority_queue<YenNewPath*, vector<YenNewPath*>, YenNewPathcmp> newq;// queue of new path
			int numOfCri = 0;
			for(int i = 0; i < v.size(); i++)if(v[i].isCritical)numOfCri++;

			for(int i = 0; i < v[pre].e.size(); i++){
				if(valid[v[pre].e[i].to] == false)continue;
				if(v[pre].e[i].edgeID == tmp.edge[tmp.x])continue;
				if(dis[v[pre].e[i].to].val == INF)continue;
				YenPath* newstate = new YenPath();
				makeNewstate(end, newstate, &v[pre].e[i], tmp, dis, criq);
				newstate -> numCri = -1;
				for(int j = 0; j < newstate -> node.size(); j++)
					if(v[newstate -> node[j]].isCritical)newstate -> numCri++;
					
				int numOfIn = 0;
				for(int j = 0; j < newstate -> node.size(); j++)
					numOfIn += reEdge[newstate->node[j]].e.size();
				
				if(v.size() <= 50)newstate->h = newstate->totalLen;
				else newstate -> h =  (double)newstate -> totalLen/newstate -> numCri
						+ v.size() * 4.5 *
						(double)(numOfCri - newstate -> numCri) / 
						(v.size() - newstate -> node.size());
						//+ (double)numOfIn //* v.size() / 100.0
						//+ newstate -> totalLen;
						//+ v.size() * 0.5 * 
						//(1.0 - (double)newstate -> numCri / numOfCri);
				if(newstate->numCri + 1 == numOfCri)newstate->h = 0;
				
				q.push(newstate);
				/*
				for(int j = 0; j < newstate -> node.size(); j++)
					occ[newstate->node[i]]++;
				count++;
				*/	
				//---------------------------------------------
				if(v.size() >= 100){
				if(constdis[v[pre].e[i].to].val == INF)continue;
				newstate = new YenPath();
				makeNewstate(end, newstate, &v[pre].e[i], tmp, constdis, criq);
				newstate -> numCri = -1;
				for(int j = 0; j < newstate -> node.size(); j++)
					if(v[newstate -> node[j]].isCritical)newstate -> numCri++;
					
				numOfIn = 0;
				for(int j = 0; j < newstate -> node.size(); j++)
					numOfIn += reEdge[newstate->node[j]].e.size();
				
				if(v.size() <= 50)newstate->h = newstate->totalLen;
				else newstate -> h =  (double)newstate -> totalLen/newstate -> numCri
						+ v.size() * 4.5 *
						(double)(numOfCri - newstate -> numCri) / 
						(v.size() - newstate -> node.size());
						//+ (double)numOfIn //* v.size() / 100.0
						//+ 0 * newstate -> totalLen;
						//+ v.size() * 0.5 * 
						//(1.0 - (double)newstate -> numCri / numOfCri);
				if(newstate->numCri + 1 == numOfCri)newstate->h = 0;
				//printf("%d\n", numOfIn);
				
				q.push(newstate);
				/*
				for(int j = 0; j < newstate -> node.size(); j++)
					occ[newstate->node[i]]++;
				count++;
				*/	
				}
				/*//----------------------------------------------------
				if(v.size() >= 100 && turnon == 1){
				if(cutvalid[v[pre].e[i].to] == false)continue;
				if(v[pre].e[i].edgeID == tmp.edge[tmp.x])continue;
				if(cutdis[v[pre].e[i].to].val == INF)continue;
				newstate = new YenPath();
				makeNewstate(end, newstate, &v[pre].e[i], tmp, cutdis, criq);
				newstate -> numCri = -1;
				for(int j = 0; j < newstate -> node.size(); j++)
					if(v[newstate -> node[j]].isCritical)newstate -> numCri++;
					
				int numOfIn = 0;
				for(int j = 0; j < newstate -> node.size(); j++)
					numOfIn += reEdge[newstate->node[j]].e.size();
				
				if(v.size() <= 50)newstate->h = newstate->totalLen;
				else newstate -> h =  (double)newstate -> totalLen/newstate -> numCri
						+ v.size() * 4.5 *
						(double)(numOfCri - newstate -> numCri) / 
						(v.size() - newstate -> node.size());
						//+ (double)numOfIn //* v.size() / 100.0
						//+ newstate -> totalLen;
						//+ v.size() * 0.5 * 
						//(1.0 - (double)newstate -> numCri / numOfCri);
				if(newstate->numCri + 1 == numOfCri)newstate->h = 0;
				
				q.push(newstate);
				
				for(int j = 0; j < newstate -> node.size(); j++)
					occ[newstate->node[i]]++;
				count++;
				}*/
				
			}// find all the deviation states
/*
			while(!newq.empty() && ((flag == true && newq.top() -> h < tmp.h) || 

				(flag == false && newq.top() -> h <= tmp.h))){
					//delete newq.top(); 
					newq.pop(); 
			}

			// ignore repeat states

			if(!newq.empty()){

				YenNewPath & curP = *newq.top();   //delete newq.top();
				newq.pop();
				YenPath* newstate = new YenPath();
				makeNewstate(end, newstate, curP, tmp, dis);

				q.push(newstate);
				//delete &curP;
				// add the shortest new path to q
								

				int lasth = curP.h;
				while(!newq.empty() && newq.top() -> h == lasth){
					YenNewPath & curP = *newq.top();   //delete newq.top();   
					newq.pop();

					newstate = new YenPath();
					makeNewstate(end, newstate, curP, tmp, dis);
					q.push(newstate);
					//delete &curP;
				}// add other new path with the same h val to q
			}*/
			tmp.pre--;   tmp.x--;
		}
		flag = false;
		//delete &tmp;
	}
	if(ansflag == false){
		edgepath.clear();
		edgepath.clear();
	}else{
		int l = 0;
		l = ansq.size();
		for(int i = 0; i < l; i++){
			edgepath.push_back(ansq.top());
			ansq.pop();
		}
	}
	// ans
	//printf("%d\n", numans);
}

struct bettercmp{
	bool operator()(YenPath* a, YenPath* b){
		return a->totalLen > b->totalLen;
	}
};

void makeNewPart(int from, int to, YenPath* newstate, std::vector<SPath> & dis, bool valid[]){
	valid[from] = 0;
	int curver = dis[from].pre;
	newstate -> edge.push_back(dis[from].preEdge);
	while(curver != to){
		valid[curver] = 0;
		newstate -> node.push_back(curver);
		newstate -> edge.push_back(dis[curver].preEdge);
		curver = dis[curver].pre;
	}
	newstate -> node.push_back(to);
}

void getlen(YenPath* newstate, Map & omap){
	int last = newstate->node[0];
	newstate->totalLen = 0;
	for(int i = 1; i < newstate->node.size(); i++){
		for(int j = 0; j < omap[last].e.size(); j++){
			if(omap[last][j].edgeID == newstate->edge[i]){
				newstate->totalLen += omap[last][j].val;
				break;
			}
		}
		last = newstate->node[i];
	}
}

void betterPath(int start, int end, Map & omap, Map & reEdge, std::vector<YenPath> & cur, YenPath & edgepath, double time){
	if(cur.size() == 0)return;
	clock_t be = clock();
	priority_queue<YenPath*, deque<YenPath*>, bettercmp> q;
	edgepath = cur[0];
	for(int i = 0; i < cur.size(); i++)q.push(&cur[i]);
	while(!q.empty()){
		YenPath & tmp = *q.top();	q.pop();
		if(tmp.totalLen < edgepath.totalLen){
			//delete &edgepath;
			edgepath = tmp;
		}// update ans
		
		clock_t ed = clock();
		if((ed - be) / CLOCKS_PER_SEC > time)return;
		
		int last1Cri = 0;
		while(!omap[tmp.node[last1Cri]].isCritical)last1Cri++;
		int cur1Cri = last1Cri + 1;
		while(!omap[tmp.node[cur1Cri]].isCritical)cur1Cri++;
		if(tmp.node[cur1Cri] == end)return;
		int cur2Cri = cur1Cri + 1;
		while(!omap[tmp.node[cur2Cri]].isCritical)cur2Cri++;
		if(tmp.node[cur2Cri] == end)return;
		int next2Cri = cur2Cri + 1;
		while(!omap[tmp.node[next2Cri]].isCritical)next2Cri++;
		// find last and next Cri, init
		
		bool valid[omap.v.size() + 5];
		while(1){
			for(int i = 0; i < omap.v.size(); i++)valid[i] = 1;
			for(int i = 0; i < tmp.node.size(); i++)valid[tmp.node[i]] = 0;
			int next1Cri = cur1Cri + 1;
			while(!omap[tmp.node[next1Cri]].isCritical)next1Cri++;
			int last2Cri = cur2Cri - 1;
			while(!omap[tmp.node[last2Cri]].isCritical)last2Cri--;
			if(next1Cri == cur2Cri){
				for(int i = last1Cri; i <= next2Cri; i++)valid[tmp.node[i]] = 1;
				YenPath* newstate = new YenPath;
				for(int i = 0; i <= last1Cri; i++){
					newstate->node.push_back(tmp.node[i]);
					newstate->edge.push_back(tmp.edge[i]);
				}
				
				std::vector<SPath> dis;
				valid[tmp.node[cur1Cri]] = 0;
				valid[tmp.node[next2Cri]] = 0;
				reEdge.shortestPath(tmp.node[cur2Cri], dis, valid);
				valid[tmp.node[cur1Cri]] = 1;
				valid[tmp.node[next2Cri]] = 1;
				if(dis[tmp.node[last1Cri]].val == INF){
					delete newstate;
					goto jump;
				}
				makeNewPart(tmp.node[last1Cri], tmp.node[cur2Cri], newstate, dis, valid);
				
				valid[tmp.node[next2Cri]] = 0;
				reEdge.shortestPath(tmp.node[cur1Cri], dis, valid);
				valid[tmp.node[next2Cri]] = 1;
				if(dis[tmp.node[cur2Cri]].val == INF){
					delete newstate;
					goto jump;
				}
				makeNewPart(tmp.node[cur2Cri], tmp.node[cur1Cri], newstate, dis, valid);

				reEdge.shortestPath(tmp.node[next2Cri], dis, valid);
				if(dis[tmp.node[cur1Cri]].val == INF){
					delete newstate;
					goto jump;
				}
				makeNewPart(tmp.node[cur1Cri], tmp.node[next2Cri], newstate, dis, valid);
				
				for(int i = next2Cri + 1; i < tmp.node.size(); i++){
					newstate->node.push_back(tmp.node[i]);
					newstate->edge.push_back(tmp.edge[i]);
				}
				
				getlen(newstate, omap);
				if(newstate->totalLen > tmp.totalLen){
					delete newstate;
					goto jump;
				}
				q.push(newstate);
				if(newstate->totalLen < edgepath.totalLen){
					edgepath = *newstate;
				}
				ed = clock();
				if((ed - be) / CLOCKS_PER_SEC > time)return;
			}else{
				for(int i = last1Cri; i <= next1Cri; i++)valid[tmp.node[i]] = 1;
				for(int i = last2Cri; i <= next2Cri; i++)valid[tmp.node[i]] = 1;
				YenPath* newstate = new YenPath;
				for(int i = 0; i <= last1Cri; i++){
					newstate->node.push_back(tmp.node[i]);
					newstate->edge.push_back(tmp.edge[i]);
				}
				
				std::vector<SPath> dis;
				valid[tmp.node[cur1Cri]] = 0;
				valid[tmp.node[next1Cri]] = 0;
				valid[tmp.node[last2Cri]] = 0;
				valid[tmp.node[next2Cri]] = 0;
				reEdge.shortestPath(tmp.node[cur2Cri], dis, valid);
				valid[tmp.node[cur1Cri]] = 1;
				valid[tmp.node[next1Cri]] = 1;
				valid[tmp.node[last2Cri]] = 1;
				valid[tmp.node[next2Cri]] = 1;
				if(dis[tmp.node[last1Cri]].val == INF){
					delete newstate;
					goto jump;
				}
				makeNewPart(tmp.node[last1Cri], tmp.node[cur2Cri], newstate, dis, valid);
				
				valid[tmp.node[cur1Cri]] = 0;
				valid[tmp.node[last2Cri]] = 0;
				valid[tmp.node[next2Cri]] = 0;
				reEdge.shortestPath(tmp.node[next1Cri], dis, valid);
				valid[tmp.node[cur1Cri]] = 1;
				valid[tmp.node[last2Cri]] = 1;
				valid[tmp.node[next2Cri]] = 1;
				if(dis[tmp.node[cur2Cri]].val == INF){
					delete newstate;
					goto jump;
				}
				makeNewPart(tmp.node[cur2Cri], tmp.node[next1Cri], newstate, dis, valid);
				valid[tmp.node[next1Cri]] = 0;
				
				for(int i = next1Cri + 1; i <= last2Cri; i++){
					newstate->node.push_back(tmp.node[i]);
					newstate->edge.push_back(tmp.edge[i]);
				}
				
				valid[tmp.node[next2Cri]] = 0;
				reEdge.shortestPath(tmp.node[cur1Cri], dis, valid);
				valid[tmp.node[next2Cri]] = 1;
				if(dis[tmp.node[last2Cri]].val == INF){
					delete newstate;
					goto jump;
				}
				makeNewPart(tmp.node[last2Cri], tmp.node[cur1Cri], newstate, dis, valid);
				
				reEdge.shortestPath(tmp.node[next2Cri], dis, valid);
				if(dis[tmp.node[cur1Cri]].val == INF){
					delete newstate;
					goto jump;
				}
				makeNewPart(tmp.node[cur1Cri], tmp.node[next2Cri], newstate, dis, valid);
				
				for(int i = next2Cri + 1; i < tmp.node.size(); i++){
					newstate->node.push_back(tmp.node[i]);
					newstate->edge.push_back(tmp.edge[i]);
				}
				
				getlen(newstate, omap);
				if(newstate->totalLen >= tmp.totalLen){
					delete newstate;
					goto jump;
				}
				q.push(newstate);
				if(newstate->totalLen < edgepath.totalLen){
					edgepath = *newstate;
				}
				ed = clock();
				if((ed - be) / CLOCKS_PER_SEC > time)return;
			}
			// newstate
			
			jump:
			if(tmp.node[next2Cri] == end){
				if(next1Cri == cur2Cri)break;
				last1Cri = cur1Cri;
				cur1Cri = next1Cri;
				cur2Cri = cur1Cri + 1;
				while(!omap[tmp.node[cur2Cri]].isCritical)cur2Cri++;
				next2Cri = cur2Cri + 1;
				while(!omap[tmp.node[next2Cri]].isCritical)next2Cri++;
			}else{
				cur2Cri = next2Cri;
				next2Cri = cur2Cri + 1;
				while(!omap[tmp.node[next2Cri]].isCritical)next2Cri++;
			}
			
		}
		//delete &tmp;
	}
}

void makeNewstate_v2(int start, int end, int tar, YenPath* newstate, YenPath & tmp, std::vector<SPath> & dis, std::vector<SPath> & predis){
	newstate -> node.clear();
	for(int i = 0; i <= tmp.pre; i++)newstate -> node.push_back(tmp.node[i]);
	newstate -> edge.clear();
	for(int i = 0; i <= tmp.pre; i++)newstate -> edge.push_back(tmp.edge[i]);

	std::vector<int> node;
	std::vector<int> edge;
	int curver = tar;
	while(curver != start){
		node.push_back(curver);
		edge.push_back(predis[curver].preEdge);
		curver = predis[curver].pre;
	}
	for(int i = node.size() - 1; i >= 0 ; i--){
		newstate -> node.push_back(node[i]);
		newstate -> edge.push_back(edge[i]);
	}

	curver = dis[tar].pre;
	newstate -> edge.push_back(dis[tar].preEdge);
	while(curver != end){
		newstate -> node.push_back(curver);
		newstate -> edge.push_back(dis[curver].preEdge);
		curver = dis[curver].pre;
	}
	newstate -> node.push_back(end);
	
	newstate -> Pre = -1;	newstate -> X = -1;
	for(int i = tmp.pre; i < newstate -> node.size(); i++)
		if(i < tmp.node.size() && newstate -> node[i] != tmp.node[i]){
			newstate -> Pre = newstate -> node[i - 1];	
			newstate -> X = newstate -> node[i];
			break;
		}
	newstate -> totalLen = tmp.len + predis[tar].val + dis[tar].val;
	int tmph = newstate -> totalLen;
	/*if(!criq.empty()){

		std::set<YenCri, YenCricmp>::iterator it = criq.begin();
		if(tmph < it -> lenToT)tmph = it -> lenToT;
	}*/
	newstate -> h = tmph;
}

void Map::criPath(int start, int end, Map & reEdge, YenPath & edgepath, double time){
	std::vector<SPath> dis;
	bool valid[v.size() + 5];
	for(int i = 0; i < v.size(); i++)valid[i] = true;
	clock_t be = clock();
	
	reEdge.shortestPath(end, dis, valid);
	if(dis[start].val == INF){
		edgepath.node.clear();
		edgepath.edge.clear();
		return;
	}

	/*for(int i = 0; i < v.size(); i++){
		if(v[i].isCritical && i != start && i != end)valid[i] = false;else valid[i] = true;
	}
	reEdge.shortestPath(end, dis, valid);*/
	
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

	priority_queue<YenPath*, deque<YenPath*>, Yencmp> q;
	q.push(&init);
	bool flag = true; // whether is operating the shortest path
	bool ansflag = false; // whether path is found
	int numans = 0; // number of ans
	priority_queue<YenPath, deque<YenPath>, Yenanscmp> ansq;

	while(!q.empty()){
		YenPath & tmp = *q.top(); 
		q.pop();
		if(realcheck(start, end, *this, tmp)){
			ansflag = true;
			numans++;	ansq.push(tmp);
			if(numans >= 10000)break;
		}
		
		clock_t ed = clock();
		if((ed - be) / CLOCKS_PER_SEC > time)break;
		
		for(int i = 0; i < v.size(); i++)valid[i] = true;
		for(int i = 0; i < tmp.node.size(); i++)
			if(tmp.node[i] != end)valid[tmp.node[i]] = false;
		// delete invalid nodes
		
		reEdge.shortestPath(end, dis, valid); // get shortest path from end

		std::set<YenCri, YenCricmp> criq;
		/*for(int i = 0; i < v.size(); i++)
			if(v[i].isCritical && valid[i]){
				YenCri tmp;	tmp.x = i;	tmp.lenToT = dis[i].val;
				criq.insert(tmp);
			}*/
		
		tmp.len = tmp.totalLen;   tmp.x = tmp.node.size();   tmp.x--;   tmp.pre = tmp.x - 1;
		int lastCri = end;
		// set init value

		while((flag == true && tmp.node[tmp.x] != tmp.Pre) || 
		(flag == false && tmp.node[tmp.pre] != tmp.Pre)){
			int x = tmp.node[tmp.x];	
			int pre = tmp.node[tmp.pre];
			if(valid[x] == false){
				valid[x] = true;
				reEdge.updateSPath(x, end, dis, valid, *this, criq);
			}// update shortest path after adding new node
			for(int i = 0; i < v[pre].e.size(); i++)
				if(v[pre].e[i].edgeID == tmp.edge[tmp.x]){
					tmp.len -= v[pre].e[i].val;   break;
				}// update tmp.len : from start to pre
			
			/*std::set<YenCri, YenCricmp>::iterator it = criq.end();	it--;
			if(!criq.empty() && it -> lenToT >= INF){
				tmp.x--;	tmp.pre--;
				continue;
			}*/
			int maxn = 0;
			for(int i = 0; i < v.size(); i++)if(valid[i] && v[i].isCritical){
				if(maxn < dis[i].val)maxn = dis[i].val;
			}
			if(maxn >= INF){
				tmp.x--;	tmp.pre--;	continue;
			}

			//priority_queue<YenNewPath*, vector<YenNewPath*>, YenNewPathcmp> newq;// queue of new path

			if(!v[pre].isCritical){
				tmp.x--;	tmp.pre--;	continue;
			}

			std::vector<SPath> predis;
			shortestPath(pre, predis, valid);

			maxn = 0;
			for(int i = 0; i < v.size(); i++)if(valid[i] && v[i].isCritical){
				if(maxn < predis[i].val)maxn = predis[i].val;
			}
			if(maxn >= INF){
				tmp.x--;	tmp.pre--;	continue;
			}

			bool visCri[v.size() + 5];
			int numOfCri = 0;
			for(int i = 0; i < v.size(); i++){
				visCri[i] = 0;
				if(v[i].isCritical)numOfCri++;
			}
			for(int k = 1; k <= 2;){
				int tmpCri = -1, minn = INF;
				for(int i = 0; i < v.size(); i++){
					if(v[i].isCritical && !visCri[i] && minn > dis[i].val + predis[i].val){
						minn = dis[i].val + predis[i].val;	tmpCri = i;
					
					}
				}
				int i = tmpCri;
				if(i == -1 || minn == INF)break;
				visCri[i] = 1;
				if(valid[i] == false)continue;
				if(i == lastCri)continue;
					
				int curver = i;
				while(curver != pre){
					if(curver != i)valid[curver] = 0;
					curver = predis[curver].pre;
				}
				std::vector<SPath> diss;
				reEdge.shortestPath(end, diss, valid);
				curver = i;
				while(curver != pre){
					if(curver != i)valid[curver] = 1;
					curver = predis[curver].pre;
				}
				if(diss[i].val == INF)continue;
					
					YenPath* newstate = new YenPath();
					makeNewstate_v2(pre, end, i, newstate, tmp, diss, predis);
					if(newstate -> X == -1){
						delete newstate;	continue;
					}
					bool occur[v.size() + 5], f = 0;
					for(int j = 0; j < v.size(); j++)occur[j] = 0;
					for(int j = 0; j < newstate -> node.size(); j++)
						if(!occur[newstate -> node[j]])
							occur[newstate -> node[j]] = true;
						else{
							f = 1;	break;
						}
					if(f == 1){
						delete newstate;	continue;
					}
					
					newstate -> numCri = -1;
					for(int j = 0; j < newstate -> node.size(); j++)
						if(v[newstate -> node[j]].isCritical)
							newstate -> numCri++;
							
					int numOfIn = 0;
					for(int j = 0; j < newstate -> node.size(); j++)
						numOfIn += reEdge[newstate->node[j]].e.size();
					
					if(v.size() <= 50)newstate->h = newstate->totalLen;
					else newstate->h = (double)newstate ->totalLen/newstate -> numCri
							+ (double)(numOfCri - newstate -> numCri) * 
							v.size() * 4.5 / 
							(v.size() - newstate -> node.size());
							//+ newstate->totalLen;
							
					if(newstate->numCri + 1 == numOfCri)newstate->h = 0;
					
					q.push(newstate);
					k++;
			}
			/*
			for(int i = 0; i < v[pre].e.size(); i++){
				if(valid[v[pre].e[i].to] == false)continue;
				if(v[pre].e[i].edgeID == tmp.edge[tmp.x])continue;
				if(dis[v[pre].e[i].to].val == INF)continue;
				YenPath* newstate = new YenPath();
				makeNewstate(end, newstate, &v[pre].e[i], tmp, dis, criq);
				newstate -> numCri = -1;
				for(int j = 0; j < newstate -> node.size(); j++)
					if(v[newstate -> node[j]].isCritical)newstate -> numCri++;
				newstate -> h =  (double)newstate -> totalLen/newstate -> numCri;
				// h fuction may need change
				if(newstate -> numCri < tmp.numCri)delete newstate;
				else 
					q.push(newstate);
			}// find all the deviation states
/*
			while(!newq.empty() && ((flag == true && newq.top() -> h < tmp.h) || 
				(flag == false && newq.top() -> h <= tmp.h))){
					//delete newq.top(); 
					newq.pop(); 
			}
			// ignore repeat states

			if(!newq.empty()){
				YenNewPath & curP = *newq.top();   //delete newq.top();
				newq.pop();
				YenPath* newstate = new YenPath();
				makeNewstate(end, newstate, curP, tmp, dis);
				q.push(newstate);
				//delete &curP;
				// add the shortest new path to q
								
				int lasth = curP.h;
				while(!newq.empty() && newq.top() -> h == lasth){
					YenNewPath & curP = *newq.top();   //delete newq.top();   
					newq.pop();
					newstate = new YenPath();
					makeNewstate(end, newstate, curP, tmp, dis);
					q.push(newstate);
					//delete &curP;
				}// add other new path with the same h val to q
			}*/
			lastCri = pre;
			tmp.pre--;   tmp.x--;
		}
		flag = false;
		//delete &tmp;
	}
	if(ansflag == false){
		edgepath.node.clear();
		edgepath.edge.clear();
	}else{
		edgepath = ansq.top();
	}
	// ans
	//printf("%d\n", numans);
}

int Map::pathInSCC(int start, int end, std::vector<int> & path){// path store ID of edge
	/*std::vector< Map > D;
	divideByDominator(start, end, D);
	path.clear();
	for(int i = 0; i < D.size(); i++){
		Map reEdge;
		D[i].reverse(reEdge);
		D[i].criPath(D[i].s[0], D[i].t[0], reEdge, path);
	}*/
	return 0;
}
