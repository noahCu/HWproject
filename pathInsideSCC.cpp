#include"lib.h"
#include<unordered_set>
//using namespace std;

Edge * Map::P2R( Edge* P ) {
	for( auto itr = (*reEdge)[ P->to ].begin(); itr != (*reEdge)[ P->to ].end(); itr++) {
		if (itr->edgeID == P->edgeID) return &(*itr);
	}
	return NULL;
}

Edge * Map::R2P( Edge* R ) {
	for (auto itr = v[ R->to ].begin(); itr != v[ R->to ].end(); itr++) {
		if (itr->edgeID == R->edgeID) return &(*itr);
	}
	return NULL;
}

void Map::reverse(){
	this->reEdge = new Map;
	Map & reEdge = *(this->reEdge);
	for(int i = 0; i < v.size(); i++){
		reEdge.push_back(v[i].ID);
		reEdge[i].isCritical = v[i].isCritical;
		reEdge[i].isDomin = v[i].isDomin;
		reEdge[i].e.clear();
	}
	
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

void Map::SPFA( int start, const std::vector< bool > & valid, std::vector<SPath> & dis) {
	std::queue< int > q;
	std::vector< bool > vis( dis.size(), 0);

	q.push( start );
	vis[start] = 1;

	while ( !q.empty() ) {
		int now = q.front();
		q.pop();
		for (auto itr = v[ now ].begin(); itr != v[ now ].end(); itr++) {
			int newval = itr->val + dis[itr->from].val;
			int to = itr->to;
			if (valid[ to ] == 0) continue;
			if ( newval < dis[ to ].val ) {
				dis[ to ].val = newval;
				dis[ to ].e = &(*itr);
				if ( vis[ to ] == 0 ) {
					vis[ to ] = 1;
					q.push( to );
				}
			}
		}
		vis[ now ] = 0;
	}
}

void YenPath::addPath( const std::vector<SPath> & dis, int start ) {
	while ( dis[ start ].e != NULL ) {
		e.push_back( dis[ start ].e );
	       start = dis[ start ].e->from;
	}
}	


void Map::shortestPath( int start, const std::vector<bool> & valid, std::vector<SPath> &dis ) {
	for (auto itr = dis.begin(); itr != dis.end(); itr++ ) itr->val = INF;
	dis[ start ].val = 0;
	dis[ start ].e = NULL;

	SPFA( start, valid, dis );
}

YenPath Map::Init(std::vector<SPath> & dis, std::vector<bool> & valid, int start, int end) {
	int mapsize = v.size() + 1;
	YenPath res;
	YenPath & init = res;

	for (int i = 0; i < mapsize; i++) valid[i] = true;

	reEdge->shortestPath( end, valid , dis);
	if (dis[ start ].val == INF) return res;
	init.addPath(dis, start);
	init.x = init.e.end();
	init.h = 0;
	for (auto itr = init.e.begin(); itr != init.e.end(); itr++) {
	       	*itr = R2P( *itr );
		init.h += (*itr)->val;
	}
	init.g = 0;
}

void Map::updateSPath( int newnode, int start, const std::vector<bool> & valid, std::vector<SPath> &dis ) {
	std::vector< bool > vis( v.size() );
	dis[ newnode ].e = NULL;
	dis[ newnode ].val = INF;
	for (auto itr = v[ newnode ].begin(); itr != v[ newnode ].end(); itr++) {
		if (dis[ itr-> to ].val + itr->val < dis[ newnode ].val ) {
			dis[ newnode ].val = dis[ itr->to ].val + itr->val;
			dis[ newnode ].e = &(*itr) ;
		}
	}
	dis[ newnode ].e = P2R( dis[ newnode ].e );
	
	reEdge->SPFA( newnode, valid, dis );
}

std::vector< YenPath* > Map::makeState( YenPath & oldPath ) {
	std::vector< YenPath* > res;
	std::vector< bool > valid( v.size(), 1 );
	

	for (auto itr = oldPath.e.begin(); itr != oldPath.e.end(); itr++) valid[ (*itr)->from ] = 0;
	
	std::vector< Edge* >::iterator & x = oldPath.x;
	auto oldx = x;
	auto lastx = oldPath.e.end() - 1;
	int end = (*lastx)->to;
	std::vector<SPath> dis( v.size() );

	int len;
	for (x = lastx; x != oldx; x--) {
		if (x == lastx) {
			reEdge->shortestPath( end, valid, dis );
			len = 0;
			for (auto itr = oldPath.e.begin(); itr != oldPath.e.end(); itr++ ) len += (*itr)->val;
		} else {
			valid[ (*x)->to ] = 1;
			updateSPath( (*x)->to, end, valid, dis );
		}

		len -= (*x)->val;

		for (auto itr = v[ (*x)->from ].begin(); itr != v[ (*x)->from ].end(); itr++) {	
			if ( itr->edgeID == (*x)->edgeID) continue;
			if ( valid[ itr->to ] == 0) continue;
			if ( dis[ itr->to ].val == INF) continue;
			
			YenPath *_YenPath = new YenPath;
			YenPath & newPath = *_YenPath;
			//newPath.e.insert( newPath.e.begin(), oldPath.e.begin(), x );
			for (auto itrr = oldPath.e.begin(); itrr != x; itrr++) newPath.e.push_back( *itrr);
			newPath.e.push_back( &(*itr) );
			newPath.g = len + itr->val;
			newPath.h = dis[ itr->to ].val;
			newPath.addPath( dis, itr->to );
			newPath.x = newPath.e.begin() + ( x - oldPath.e.begin() );
			
			for (auto itrr = newPath.e.end() - 1; itrr != newPath.x; itrr--) 
				*itrr = R2P( *itrr );
			res.push_back( _YenPath );
		}
		if (x == oldPath.e.begin()) break;
	}

	return res;

}

bool Map::checkValid( const std::vector<int> &criList, const YenPath & path ) {
	std::unordered_set< int > vis;
	for ( auto itr = path.e.begin(); itr != path.e.end(); itr++ ) {
		vis.insert( (*itr)->to );
	}
	if ( !path.e.empty() ) vis.insert( (*path.e.begin())->from );
	for (auto itr = criList.begin(); itr != criList.end(); itr++) if ( vis.find( *itr ) == vis.end() ) return 0;
	return 1;
}


		
class Yencmp{
	public:
		bool operator() (const YenPath* a, const YenPath* b) {
			return (a->g + a->h) > (b->g + b->h);
		}
};
		

YenPath Map::criPath( int start, int end) {
	
	YenPath res;
	std::vector<SPath> dis( v.size() );
	int mapsize = v.size();
	std::vector<bool> valid( mapsize, 0 );
	reEdge->shortestPath(end, valid, dis);
	YenPath * _init = new YenPath;
	YenPath & init = * _init;
	init = Init(dis, valid, start, end);
	if (init.g == INF || init.h == INF) return init;

	//Create critical list
	std::vector<int> criList;
	int id = 0;
	for (auto itr = v.begin(); itr != v.end(); itr++, id++) if (itr->isCritical) criList.push_back( id );

	if ( start == end && criList.size() > 1) return res; 
	std::priority_queue< YenPath*, std::vector< YenPath* >, Yencmp > q;
	q.push( &init );

	while ( !q.empty() ) {
		YenPath & tmp = *q.top();
		q.pop();
		if ( checkValid( criList, tmp ) ) {
			res = tmp;
			break;
		}
		std::vector< YenPath* > newState = makeState( tmp );
	
		for (auto itr = newState.begin(); itr != newState.end(); itr++) 
			q.push( *itr );
		delete &tmp;
	}
	
	return res; 
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
	reverse();
	return 0;
}

