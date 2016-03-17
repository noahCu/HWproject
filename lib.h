#ifndef _lib
#define _lib
#include<vector>
#include<map>
#include<set>
#include<string>

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
		Vertex( int x );
		Edge & operator[]( int x ); // vertex[x] would be vertex.e[x]
		int degree();
		int push_back( Edge newEdge );
		int ID;
		std::vector<Edge> e;
	        bool isCritical;	
	private:
};
	

class Map{
	public:
		Vertex & operator[](int x); // map[x] would be map.v[x]
		void divideByDominator( std::vector< Map > & D, Map & Dmap );// both D and Dmap should be empty
		int push_back( int x );
		int N, M;
		int s, t;
		std::vector<Vertex> v;
	private:
};

void setSCC( const Map & omap, std::vector<Map> & SCC, Map & SCCmap );
const std::vector<int> & findPath( const std::vector<Map> & SCC, const Map & SCCmap , std::map<int, bool> isCritical);

void inputMap( std::string mapfile, std::string criticalfile, Map & omap, std::map<int, bool> & isCritical);
void outputRes( const std::vector<int> & path, const std::string & outputfile);

#endif
