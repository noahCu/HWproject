#ifndef _lib
#define _lib
#include<vector>
#include<map>
#include<set>
#include<string>
#include<queue>
#include<set>
#define INF 100000

class SPath{
	public:
		int pre;
		int val;
		int x;
		int preEdge;
	private:
};

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
		const Edge & operator[] (int x) const;
		std::vector<Edge>::iterator  begin();
		std::vector<Edge>::iterator  end();
		const std::vector<Edge>::const_iterator begin() const;
		const std::vector<Edge>::const_iterator end() const;
		int size() const;
		int degree();
		int push_back( Edge newEdge );
		int ID;
		std::vector<Edge> e;
	    bool isCritical;	
	    bool isDomin;
	private:
};
	
class YenPath{
	public:
		std::vector<int> node;
		std::vector<int> edge; // store edgeID before node[i]
		int x; // index in node[]
		int pre; // index in node[]
		int len; // from start to pre
		int totalLen; // from start to end
		int X, Pre; // X and Pre of this state, vertex, not index in node[]
		double h; // val of A* function
		int numCri; // number of critical
	private:
};

struct Yencmp{
	bool operator()(YenPath* a, YenPath* b){
		//if(a -> numCri != b -> numCri)return a -> numCri > b -> numCri;
		return a -> h > b -> h;
	}
};

class YenNewPath{
	public:
		int x;
		int edgeID;
		int h;
		int edgeLen;
	private:
};

struct YenNewPathcmp{
	bool operator()(YenNewPath* a, YenNewPath* b){
		return a -> h > b -> h;
	}
};

class YenCri{
	public:
		int x;    int lenToT;
	private:
};

struct YenCricmp{
	bool operator()(YenCri a, YenCri b){
		return a.lenToT < b.lenToT; 
	}
};

class Map{
	public:
		Map();
		Vertex & operator[](int x); // map[x] would be map.v[x]
		const Vertex & operator[](int x) const;
		void divideByDominator(int start, int end, std::vector< Map > & D);// both D and Dmap should be empty
		int push_back( int x );
		int push_back( const Vertex & );
		void findDominator(int start, int end);
		bool findDominatorPath(int x, int end, bool vis[], std::vector<int> & path);
		int traverse(int x, int end, int vis[], Map & newD);
		int pathInSCC(int start, int end, std::vector<int> & path);
		void reverse(Map & reEdge);
		void shortestPath(int start, std::vector<SPath> & dis, bool valid[]);
		void criPath(int start, int end, Map & reEdge, YenPath & edgepath, double time);
		void criPathold(int start, int end, Map & reEdge, std::vector<YenPath> & edgepath);
		bool checkValid(int start, int end, std::vector<int> & path);
		void updateSPath(int newnode, int start, std::vector<SPath> & dis, bool valid[], Map & reEdge, std::set<YenCri, YenCricmp> & criq);
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
		const Edge & getOutter(int, int) const;
		const Edge & getInner(int, int ) const;
		int push_back(int);
		int push_back(const Vertex &);
	private:
		std::vector<Vertex> innerEdge;
};



void setSCC( const Map & omap, std::vector<Map> & SCC, BigMap & SCCmap );
std::vector<int> indPath( std::vector<Map> & SCC, const Map & SCCmap);

int inputMap( std::string mapfile, std::string criticalfile, Map & omap, std::map<int, bool> & isCritical);
void outputRes( const std::vector<int> & path, const std::string & outputfile);

void betterPath(int start, int end, Map & omap, Map & reEdge, std::vector<YenPath> & cur, YenPath & edgepath, double time);

int mainTest();
#endif
