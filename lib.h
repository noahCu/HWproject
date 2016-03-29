#ifndef _lib
#define _lib
#include<vector>
#include<map>
#include<set>
#include<string>
#include<queue>

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
		std::vector<Edge>::iterator  begin();
		std::vector<Edge>::iterator  end();
		const std::vector<Edge>::const_iterator begin() const;
		const std::vector<Edge>::const_iterator end() const;
		int degree();
		int push_back( Edge newEdge );
		int ID;
		std::vector<Edge> e;
	    bool isCritical;	
	    bool isDomin;
	private:
};
	

class Map{
	public:
		Vertex & operator[](int x); // map[x] would be map.v[x]
		const Vertex & operator[](int x) const;
		void divideByDominator(int start, int end, std::vector< Map > & D);// both D and Dmap should be empty
		int push_back( int x );
		void findDominator(int start, int end);
		bool findDominatorPath(int x, int end, bool vis[], std::vector<int> & path);
		void traverse(int x, int end, int vis[], Map & newD);
		void pathInSCC(int start, int end, std::vector<int> & path);
		void reverse(Map & reEdge);
		void shortestPath(int start, std::vector<SPath> & dis, bool valid[]);
		void criPath(int start, int end, Map & reEdge, std::vector<int> edgepath);
		int size() const;
		int N, M;
		std::vector<int> s;
		std::vector<int> t;
		std::vector<Vertex> v;
	private:
};

class BigMap: public Map {
	public:
		void addEdge( int from, int to, int ifrom, int ito, int val, int id);
	private:
		std::vector<Vertex> innerEdge;
};

class YenPath{
	public:
		vector<int> node;
		std::vector<int>::iterator x;
		std::vector<int>::iterator pre;
		int len; // from start to pre
		int totalLen; // from start to end
		int X, Pre; // X and Pre of this state
		int h; // val of A* function
	private:
};

struct Yencmp{
	bool operator()(YenPath* a, YenPath* b){
		return a -> h > b -> h;
	}
};

class SPath{
	public:
		int pre;
		int val;
		int x;
	private:
};

void setSCC( const Map & omap, std::vector<Map> & SCC, BigMap & SCCmap );
const std::vector<int> & findPath( const std::vector<Map> & SCC, const Map & SCCmap , std::map<int, bool> isCritical);

void inputMap( std::string mapfile, std::string criticalfile, Map & omap, std::map<int, bool> & isCritical);
void outputRes( const std::vector<int> & path, const std::string & outputfile);

#endif
