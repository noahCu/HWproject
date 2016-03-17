ALLO = main.o io.o graph.o
CFLAG = -c

all: HW

HW: ${ALLO}
	g++ -g ${ALLO} -o HW

main.o: main.cpp lib.h
	g++ -c main.cpp

io.o: io.cpp lib.h
	g++ -c -std=c++11 io.cpp

graph.o: graph.cpp lib.h
	g++ -c graph.cpp

clean:
	rm *.o
