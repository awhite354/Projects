#simple makefile

#define target, its dependencies and files
p7: main.o bst.o node.o
	g++ -o p7 main.o bst.o node.o

#define how each object file is to be built
main.o: main.cpp bst.h node.h
	g++ -c main.cpp

bst.o:	bst.h bst.cpp node.h
	g++ -c bst.cpp

node.o: node.h node.cpp
	g++ -c node.cpp

#clean ip
clean:	
	rm -f p7 *.o *~
