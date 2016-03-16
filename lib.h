#ifndef _lib
#define _lib
#include<vector>

class Edge{
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
		void findDominator(int s, int t);
		bool tran(int x);

	private:
		int N, M;
		bool vis[N + 5];
		std::vector< vector<Edge> > e;
		std::vector <int> domin;
}

void setSCC( Map map, vector<Map> SCC, Map SCCmap );
const vector<int> & findPath( vector<Map> SCC, Map SCCmap );

void inputMap( String inputfile, Map map);
void outputRes( const vector<int> & path, String outputfile);

#endif
