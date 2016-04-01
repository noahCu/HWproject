#include<cstdio>
#include<time.h>
#include<fstream>
#include<iostream>
#include"lib.h"
#include<string>


int display( char filename[] ) {
	std::cout<< "display content of " << filename << std::endl;
	std::cout<< "---------------\n";

	std::ifstream file;
	file.open( filename );
	while (1) {
		char ch = file.get();
		if (ch == EOF) break;
		std::cout<< ch;
	}
	std::cout<< "---------------\n";
}

int printSCC(const std::vector<Map> & SCC, const BigMap & SCCMap) {
	for (auto itr = SCC.begin(); itr != SCC.end(); itr++) {
		for (auto itrr = itr->v.begin(); itrr != itr->v.end(); itrr++ ) {
			std::cout << itrr->ID<< ',';
		}
		std::cout <<std::endl << "------------------------"<<std::endl;
	}
}

void make_data( int N, int M, int K) {
	std::vector<int> cnt(N, 0);
	std::ofstream topo;
	topo.open( "topo.csv" );
	srand( time(NULL) );
	while (M-- > 0 ) {
		int from, to, val;
		val = rand() % 10;
		do { from = rand() % N; } while ( cnt[from] > 7);
		to = rand() % N;
		topo<< M << ',' << from << ',' << to << ',' << val << std::endl;
	}
	topo.close();
	
	std::ofstream demand;
	demand.open( "demand.csv" );
	std::vector<bool> used(N, 0);
	demand << rand() % N << ',' << rand() % N << ',';
	while ( K-- ) {
		int c;
		do { c = rand() % N; } while ( used[c] );
		used[c] = 1;
		demand << c;
		if ( K != 1 ) demand<< '|';
	}

}


void testcriPath(Map & omap) {
	YenPath res;
	Map reEdge;
	omap.reverse(reEdge);
	omap.criPath( omap.s[0], omap.t[0], reEdge, res);
	if ( res.edge.empty() ) std::cout<< "no possible res\n";
	for (auto itr = res.edge.begin(); itr != res.edge.end(); itr++) std::cout<< *itr << " -> ";
}
	

int mainTest() {
	Map omap;
	std::map< int, bool> isCritical;


	//int N = 600, M = 600 * 8, K = 0;
	//std::cin >> N >> M >> K;
	//make_data( N, M, K);


	inputMap(std::string("topo.csv"), std::string("demand.csv"), omap, isCritical);

	/*
	std::vector<Map> SCC;
	BigMap SCCMap;
	setSCC ( omap, SCC, SCCMap);
	printSCC( SCC, SCCMap );
	*/

	//testcriPath(omap);

	return 0;
}
