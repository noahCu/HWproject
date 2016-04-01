#include<cstdio>
#include<time.h>
#include<fstream>
#include<iostream>
#include"lib.h"
#include<string>

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
	demand.open( "deman.csv" );
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


int mainTest() {
	Map omap;
	std::map< int, bool> isCritical;
	make_data( 600, 600 * 8 , 50);
	inputMap(std::string("topo.csv"), std::string("demand.csv"), omap, isCritical);

	std::vector<Map> SCC;
	BigMap SCCMap;
	setSCC ( omap, SCC, SCCMap);
	printSCC( SCC, SCCMap );
	return 0;
}
