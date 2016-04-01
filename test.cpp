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

void make_topo( int N, int M) {
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
}


int mainTest() {
	Map omap;
	std::map< int, bool> isCritical;
	make_topo( 600, 600 * 3 );
	inputMap(std::string("topo.csv"), std::string("demand.csv"), omap, isCritical);

	std::vector<Map> SCC;
	BigMap SCCMap;
	setSCC ( omap, SCC, SCCMap);
	printSCC( SCC, SCCMap );
	return 0;
}
