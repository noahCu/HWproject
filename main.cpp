#include<cstdio>
#include<iostream>
#include<vector>
using namespace std;

Class Edge{
	prublic:

		void set(int from, int to, int val);

	private:
		int from, to;
		int val; // path cost
		int edgeID;
}

class Map{
	public:
		operator[](int x); // map[x][y] would be map.e[x][y];

	private:
		int N, M;
		vector< vector<Edge> > e;
}

int main() {
	Map map;
	inputMap("input.csv", map);
	
	vector<Map> SCC;
	Map SCCmap;
	setSCC( map, SCC, SCCmap);
	divideByDominator( SCC, SCCmap);
	vector<int> path = findPath( SCC, SCCmap);
	
	outputRes(path);
	return 0;
}
