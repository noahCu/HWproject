#include"lib.h"

void BigMap::addEdge( int from, int to, int ifrom, int ito, int val, int id) {
	v[from].push_back( Edge( from, to, val, id ) );
	innerEdge[from].push_back( Edge( ifrom, ito, val, id ) );
}

int remove(const Map & omap, const int farA[], bool removed[], int x, int acs) {
	if (removed[x] == 1) return 0;
	if (farA[x] != acs) return 0;
	removed[x] = 1;
	for (auto itr = omap[x].begin(); itr != omap[x].end(); itr++) remove(omap, farA, removed, itr->to, acs);
	return 1;
}

int far( int farA[], int x ) {
	if (farA[x] == x) return x;
	farA[x] = far( farA, farA[x] );
	return farA[x];
}

int tarjan( const Map &omap, bool * removed, int * farA, int * height, int now, int now_height) { //return the height that vertex 'now' can reach 
	if (removed[ now ] == 1) return INF;
	if (height[now] != -1) return height[ far( farA, now ) ];
	farA[ now ] = now;
	height[now] = now_height;
	for ( auto edge = omap[now].begin(); edge != omap[now].end(); edge++) {
		if (!removed[ far( farA, edge->to ) ] && tarjan( omap, removed, farA, height, edge->to, now_height + 1) < height[ farA[now] ] ) {
			farA[ now ] = far( farA, edge->to );
		}
	}
	if (farA[ now ] == now) remove(omap, farA, removed, now, now);
	return height[ far( farA, now ) ];
}
	

std::vector<int> findBelong( const Map & omap ) {
	
	int mapsize = omap.size();
	bool removed[ mapsize ];
	int farA[ mapsize ];
	int height[ mapsize ];

	for (int i = 0; i < mapsize; i++) {
		removed[i] = 0;
		farA[i] = i;
		height[i] = -1; //height[i] = - 1 indicates that i is not visited yet.
	}

	for (int i = 0; i < mapsize; i++) if ( removed[i] == 0 )
		tarjan( omap, removed, farA, height, i, 0);

	std::map< int, int > idxRef;
	int cnt = 0;
	std::vector<int> belong;
	belong.resize( omap.size() );
	for (int i = 0; i < mapsize; i++) {
		if (idxRef.find( farA[i] ) != idxRef.end() ) {
		       belong[i] = idxRef[ farA[i] ];
		} else {
			idxRef[ farA[i] ] = cnt++;
			belong[i] = idxRef[ farA[i] ];
		}
	}
	return belong;
}

int belongMax( const std::vector<int> belong ) {
	int res =0;
	for (auto it = belong.begin(); it != belong.end(); it++) if (*it> res) res = *it;
	return res;
}

void setSCC( const Map & omap, std::vector<Map> & SCC, BigMap & SCCMap ) {
	std::vector<int> belong;
	std::vector<int> SCCid;
	belong = findBelong( omap );

	int mb = belongMax( belong ) + 1;
	SCC.resize( mb);
	for (int i = 0; i < mb; i++) SCCMap.push_back( i );
	
	std::vector<int> belong_id( omap.size() );

	int mapsize = omap.size();
	for (int i = 0; i < mapsize; i++) {
		belong_id[i] = SCC[ belong[i] ].size();
		SCC[ belong[i] ].push_back( omap[i] );
		if ( omap[i].isCritical ) SCCMap[ belong[i] ].isCritical = 1;
		if ( i == omap.s[0] ) {
			SCCMap.s[0] = belong[i];
			SCC[ belong[i] ].s[0] = belong_id[i];
		}
		if ( i == omap.t[0] ) {
			SCCMap.t[0] = belong[i];
			SCC[ belong[i] ].t[0] = belong_id[i];
		}
	}

	for (auto ver = omap.v.begin(); ver != omap.v.end(); ver++) {
		for (auto edge = ver->begin(); edge != ver->end(); edge++) {
			if (belong[ edge->from ] == belong[ edge->to ] ) {
				int mapid = belong[ edge->from ];
				SCC[ mapid ][ belong_id[ edge->from ] ].push_back( Edge( belong_id[ edge->from ], belong_id[ edge->to ], edge->val, edge->edgeID ) );
			} else {
				int mapid1 = belong[ edge->from ];
				int mapid2 = belong[ edge->to ];
				SCCMap.addEdge( mapid1, mapid2, belong_id[ edge->from ], belong_id[ edge->to ], edge->val, edge->edgeID );
			}
		}
	}
}	

int tpSort( const std::vector<Map> & SCC, const BigMap & SCCMap, std::vector< std::set<int> > tpOrder) {
	std::vector<int> degree;
	std::vector<int> criticalID;
	for (int i = SCCMap.size(); i >= 0; i--) degree[i] = 0;
	for (int i = 0; i < SCCMap.size(); i++) 
		for (int j = 0; j < SCCMap[i].size(); j++) {
			degree[ SCCMap[i][j].to ]++;
		}
	for (int i = degree.size() - 1; i >= 0; i--) if (degree[i] == 0) tpOrder[0].insert(i);

	if (tpOrder[0].size() == 0) return -1; // error:non-acyclic graph
	
	for (int i = 0; tpOrder[i].size() != 0; i++) {
		int size = tpOrder[i].size();
		criticalID[i] = -1;
		for (auto it = tpOrder[i].begin(); it!= tpOrder[i].end(); ++it) {
			if (SCCMap[ *it ].isCritical == 1) {
				if (criticalID[i] != -1 ) return -2; // error: no possible answer
				criticalID[i] = *it;
			}
			for (auto itt = SCCMap[ *it ].begin(); itt != SCCMap[ *it ].end(); it++) {
				degree[ itt->to ]--;
				if (degree[ itt->to ] == 0) tpOrder[ i + 1 ].insert( itt->to );
			}
		}
	}

	for (int i = 0; i < tpOrder.size(); i++) {
		if ( criticalID[i] != -1) {
			tpOrder[i].clear();
			tpOrder[i].insert( criticalID[i] );
		}
	}

	std::set<int> valid;
	for (int i = 0; i < tpOrder.size() - 1; i++) {
		valid.clear();
		for (auto it = tpOrder[i].begin(); it != tpOrder[i].end(); it++) {
			for ( auto itt = SCCMap[ *it ].begin(); itt!= SCCMap[ *it ].end(); itt++) if (tpOrder[i + 1].find( itt->to ) != tpOrder[i + 1].end() ) {
				valid.insert( itt->to );
			}
		}
		tpOrder[i + 1] = valid;
	}
}

int updatePath( std::vector<Map> & SCC, const BigMap & SCCMap, const std::set<int> tpOrder, const int order, std::map< int, std::map< int, std::vector<int> > > & resPath, std::map< int, std::map< int, int > > & resDis, const int now) {
	for (int i = 0; i < SCCMap[ now ].size(); i++) {
		const Edge & e = SCCMap.getInner( now, i );
		const Edge & eOut = SCCMap.getOutter( now, i );
		if ( tpOrder.find( eOut.to ) == tpOrder.end() ) continue;
		if (resDis[ now ].find( e.from ) == resDis[ now ].end() ) {
			std::vector<int> pathtemp;
			std::vector<int> & temp = pathtemp;
			resDis[ now ][ e.from ] = SCC[ now ].pathInSCC( SCC[now].s[0], e.from, pathtemp );
			resPath[ now ][ e.from ] = resPath[ now ][ -pathtemp[0] ];
			resPath[ now ][ e.from ].insert( resPath[ now ][ e.from ].end(), temp.begin(), temp.end() );
		}
		int newDis = resDis[now][ e.from ] + e.val;
		if (resDis[ eOut.to ].find( e.to ) == resDis[ eOut.to ].end() || resDis[ eOut.to ][e.to ] > newDis ) {
			resDis[ eOut.to ][ e. to ] = newDis;
			resPath[ eOut.to ][ e.to ] = resPath[now][ e.from ];
			resPath[ eOut.to ][ e.to ].push_back( e.edgeID );
			if (SCC[ eOut.to ][ SCC[ eOut.to ].size() - 1 ].ID != -2) {
				SCC[ eOut.to ].push_back(-2); // Vertex.ID == -2 means it is not a real Vertex
				SCC[ eOut.to ].s[0] =  SCC[ eOut.to ].size() - 1;
			}
			SCC[ eOut.to ][ SCC[ eOut.to ].size() - 1 ].push_back( Edge( SCC[ eOut.to ].s[0], e.to, newDis, -e.to ) );  // Edge.ID == -x means that this is not a real edge and it connects to Vertex x
		}
	}
}




 std::vector<int> findPath( std::vector<Map> & SCC, const BigMap & SCCMap) {
	std::vector< std::set<int> > tpOrder;
	tpSort( SCC, SCCMap, tpOrder );
	
	int startOrder = -1;
	int endOrder = -1;
	int s = SCCMap.s[0];
	int t = SCCMap.t[0];
	for (int i = 0; i < tpOrder.size(); i++) {
		if (tpOrder[i].find( s ) != tpOrder[i].end() ) startOrder = i;
		if (tpOrder[i].find( t ) != tpOrder[i].end() ) endOrder = i;
	}
	
	if (startOrder == -1 || endOrder == -1|| startOrder >= endOrder) return std::vector<int>(1, -1); // no possible result.
	
	std::map< int, std::map< int, std::vector<int> > > resPath; // res[a][b] is the path from s to SCC[a][b]
	std::map< int, std::map< int, int > > resDis; // res[a][b] is the path distance from s to SCC[a][b]

	updatePath( SCC, SCCMap,tpOrder[ startOrder + 1 ], startOrder, resPath, resDis, s);	

	for (int order = startOrder + 1; order < endOrder; order++) 
		for (auto mapID = tpOrder[ order ].begin(); mapID != tpOrder[ order ].end(); mapID++) {
			updatePath( SCC, SCCMap, tpOrder[ order + 1], order, resPath, resDis, *mapID );
		}
	std::vector<int> tempPath;
	SCC[t].pathInSCC( SCC[t].s[0], SCC[t].t[0], tempPath);
	std::vector<int> res = resPath[t][SCC[t].t[0]];
	res.insert( res.end(), tempPath.begin(), tempPath.end() );
	return res;
}








