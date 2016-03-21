ALLO = main.o io.o graph.o SCC.o
CFLAG = -c -std=c++11

all: HW

HW: ${ALLO}
	g++ -g ${ALLO} -o HW

main.o: main.cpp lib.h
	g++ -c main.cpp

io.o: io.cpp lib.h
	g++ -c -std=c++11 io.cpp

SCC.o: SCC.cpp lib.h
	g++ ${CFLAG} SCC.cpp

graph.o: graph.cpp lib.h
	g++ -c graph.cpp

clean:
	rm *.o
