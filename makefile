all: HW

HW: main.o io.o
	g++ -g main.o io.o -o HW

main.o: main.cpp lib.h
	g++ -c main.cpp

io.o: io.cpp lib.h
	g++ -c -std=c++11 io.cpp

clean:
	rm *.o
