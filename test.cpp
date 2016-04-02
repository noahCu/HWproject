#include<cstdio>
#include<time.h>
#include<fstream>
#include<iostream>
#include"lib.h"
#include<string>

void print( const Edge & e) {
	std::cout << "(" << e.edgeID << ", " << e.from << ", " << e.to << ", " << e.val << ")";
}

void Map::display(const YenPath & path ){
	std::cout << std::endl <<"path:" ;
	for (auto itr = path.e.begin(); itr != path.e.end(); itr++) std::cout << v[ (*itr)->from ].ID <<"->";
	std::cout<< v[ (*path.e.rbegin())->to ].ID << std::endl;
	std::cout<< "len is: "<< path.g + path.h<< std::endl;
}

int display( std::string filename ) {
	std::cout<< "display content of " << filename << std::endl;
	std::cout<< "---------------\n";

	std::ifstream file;
	file.open( filename );
	while (1) {
		char ch = file.get();
		if (ch == EOF) break;
		std::cout<< ch;
	}
	std::cout<<std::endl;
	std::cout<< "---------------\n"<< std::endl;
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
	for (int i = 0 ; i < M; i++) {
		int from, to, val;
		val = rand() % 20;
		do { from = rand() % N; } while ( cnt[from] > 7);
		to = rand() % N;
		topo<< i << ',' << from << ',' << to << ',' << val << std::endl;
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
		if ( K != 0 ) demand<< '|';
	}

}


void testcriPath(Map & omap) {
	YenPath res;
	Map reEdge;
	omap.reverse();
	res = omap.criPath( omap.s[0], omap.t[0]);
	if ( res.e.empty() ) std::cout<< "no possible res\n";
	for (auto itr = res.e.begin(); itr != res.e.end(); itr++) {
		print( **itr );
		std::cout<< "->";
	}
	std::cout<< std::endl;
}
	

int mainTest() {
	Map omap;
	std::map< int, bool> isCritical;


	int N = 600, M = 600 * 8, K = 0;
	std::cout << "make new Data ? (1/0)\n";
	int x;
	std::cin>>x;
	if (x == 1) {
		std::cin >> N >> M >> K;
		make_data( N, M, K);
	}


	inputMap(std::string("topo.csv"), std::string("demand.csv"), omap, isCritical);
	display("topo.csv" );
	display("demand.csv" );

	/*
	std::vector<Map> SCC;
	BigMap SCCMap;
	setSCC ( omap, SCC, SCCMap);
	printSCC( SCC, SCCMap );
	*/

	testcriPath(omap);

	return 0;
}
