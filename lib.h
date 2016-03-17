#ifndef _lib
#define _lib
#include<vector>
#include<map>
#include<set>

class Edge{
	public: 
		void setEdge( int from, int to, int val );
		Edge( int from, int to, int val, int edgeID );
		int from, to;
		int val; // path cost
		int edgeID; // not necessary;
	private:
};

class Vertex{ 
	public: 
		operator[]( int x ); // vertex[x] would be vertex.e[x]
		int degree();
		int ID;
		std::vector<Edge> e;
		bool isDomin;
		std::vector<Edge> e; 
	private:
};
	

class Map{
	public:
		operator[](int x); // map[x] would be map.v[x]
		void divideByDominator( vector< Map > & D, Map & Dmap );// both D and Dmap should be empty

		void findDominator();
		bool traverse(int x, bool vis[]);
		
		int N, M;
		int s, t;
		std::vector<Vertex> v;
		
	private:
};

void setSCC( const Map & omap, std::vector<Map> & SCC, Map & SCCmap );
const vector<int> & findPath( const std::vector<Map> & SCC, const Map & SCCmap );

void inputMap( String mapfile, String criticalfile, Map & omap, std::set<int> & isCritical);
void outputRes( const std::vector<int> & path, const String & outputfile, const std::set<int> & isCritical);

#endif
