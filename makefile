ALLO = main.o io.o graph.o SCC.o dominator.o test.o pathInsideSCC.o
CFLAG = -g -c -std=c++11

all: HW

HW: ${ALLO}
	g++ -g ${ALLO} -o HW

main.o: main.cpp lib.h
	g++ ${CFLAG} main.cpp

test.o: test.cpp lib.h
	g++ ${CFLAG} test.cpp

io.o: io.cpp lib.h
	g++ ${CFLAG} -std=c++11 io.cpp

SCC.o: SCC.cpp lib.h
	g++ ${CFLAG} SCC.cpp

graph.o: graph.cpp lib.h
	g++ ${CFLAG} graph.cpp

dominator.o: dominator.cpp lib.h
	g++ ${CFLAG} dominator.cpp

pathInsideSCC.o: pathInsideSCC.cpp lib.h
	g++ ${CFLAG} pathInsideSCC.cpp

clean:
	rm *.o
