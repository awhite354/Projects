# simple makefile

# define target, it's dependencies and files
p6: main.o maze.o disjointSet.o
	g++ -o p6 main.o maze.o disjointSet.o

# define how each objject file is to be built
main.o: main.cpp maze.h disjointSet.h
	g++ -c main.cpp

maze.o: maze.h maze.cpp disjointSet.h
	g++ -c maze.cpp

disjointSet.o: disjointSet.h disjointSet.cpp
	g++ -c disjointSet.cpp

# clean ip
clean: 
	rm -f p6 *.o *~
