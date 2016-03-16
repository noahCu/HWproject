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

	private:
		int N, M;
		std::vector< vector<Edge> > e;
}

void setSCC( Map map, vector<Map> SCC, Map SCCmap );
void divideByDominator( vector<Map> SCC, Map SCC );
const vector<int> & findPath( vector<Map> SCC, Map SCCmap );

void inputMap( String inputfile, Map map);
void outputRes( const vector<int> & path, String outputfile);

#endif
