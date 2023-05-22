#simple makefile

# define target, its dependencies and files
p8: main.o graph.o
	g++ -o p8 main.o graph.o

# define how each object is to be built
main.o: main.cpp graph.h
	g++ -c main.cpp

graph.o: graph.h graph.cpp
	g++ -c graph.cpp

clean:
	rm -f p8 *.o *~
