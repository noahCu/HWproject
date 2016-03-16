#ifndef _lib
#define _lib
#include<vector>

Class Edge{
	public: 
		void set( int from, int to, int val );

	private:
		int from, to;
		int val; // path cost
		int edgeID; // not necessary;
}

class Map{
	public:
		operator[](int x); // map[x][y] would be map.e[x][y]
		void divideByDominator( vector< Map > & D, Map & Dmap );// both D and Dmap should be empty

	private:
		int N, M;
		int s, t;
		std::vector< vector<Edge> > e;
}

void setSCC( Map map, vector<Map> SCC, Map SCCmap );
const vector<int> & findPath( vector<Map> SCC, Map SCCmap );

void inputMap( String mapfile, String criticalfile, Map map, map<int> isCritical);
void outputRes( const vector<int> & path, String outputfile, map<int> isCritical);

#endif
