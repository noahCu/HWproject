#include"lib.h"
#define INF 10000

void BigMap::addEdge( int from, int to, int ifrom, int ito, int val, int id) {
	v[from].push_back( Edge( from, to, val, id ) );
	innerEdge[from].push_back( Edge( ifrom, ito, val, id ) );
}

int tarjan( const Map &omap, bool * removed, int * farA, int * height, int now, int now_height) { //return the height that vertex 'now' can reach 
	if (height[now] != -1) return height[ farA[ now ] ];
	height[now] = now_height;
	for ( auto edge = omap[now].begin(); edge != omap[now].end(); edge++) {
		if (!removed[ farA[ edge->to] ] && tarjan( omap, removed, farA, height, edge->to, now_height + 1) < height[ farA[now] ] ) {
			farA[ now ] = farA[ edge->to ];
		}
	}
	if (farA[ now ] == now) removed[ now ] = 1;
	return height[ farA[ now ] ];
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

	tarjan( omap, removed, farA, height, *omap.s.begin(), 0 );

	std::map< int, int > idxRef;
	int cnt = 0;
	std::vector<int> belong;
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

void setSCC( const Map & omap, std::vector<Map> & SCC, BigMap & SCCMap ) {
	std::vector<int> belong;
	std::vector<int> SCCid;
	belong = findBelong( omap );
	std::vector<int> belong_id;

	int mapsize = omap.size();
	for (int i = 0; i < mapsize; i++) {
		belong_id[i] = SCC[ belong[i] ].size();
		SCC[ belong[i] ].push_back( omap[i].ID );
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
