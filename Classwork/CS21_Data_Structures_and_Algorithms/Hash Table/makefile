#simple makefile

#define target, its dependencies and files

p5: main.o hashTable.o hashEntry.o
	g++ -o p5 main.o hashTable.o hashEntry.o

#define how each object file is built
main.o: main.cpp hashTable.h hashEntry.o
	g++ -c main.cpp

hashTable.o: hashTable.h hashTable.cpp hashEntry.o
	g++ -c hashTable.cpp

hashEntry.o: hashEntry.h hashEntry.cpp
	g++ -c hashEntry.cpp

# clean
clean: 
	rm -f p5 *.o *~
