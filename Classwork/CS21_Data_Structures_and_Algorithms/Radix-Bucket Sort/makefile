#simple makefile

#define target, it's dependencies and files

p4: main.o
	g++ -o p4 main.o

main.o:	main.cpp
	g++ -c main.cpp

#clean ip
clean:
	rm -f p4 *.o *~
