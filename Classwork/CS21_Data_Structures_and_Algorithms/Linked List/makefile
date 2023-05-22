# simple makefile

# define target, its dependencies and files
p1: main.o queue.o list.o linknode.o
	g++ -o p1 main.o queue.o list.o linknode.o

# define how each object file is to be built
main.o: main.cpp queue.o list.h linknode.h
	g++ -c main.cpp

queue.o:queue.h queue.cpp list.h linknode.h
	g++ -c queue.cpp

list.o: list.h list.cpp linknode.h
	g++ -c list.cpp 

linknode.o: linknode.h linknode.cpp
	g++ -c linknode.cpp

# clean ip
clean:
	rm -f p1 *.o *~ 
