#makefile

p2: main.o intMinHeap.o
	g++ -o p2 main.o intMinHeap.o

main.o: main.cpp intMinHeap.h
	g++ -c main.cpp

intMinHeap.o: intMinHeap.h intMinHeap.cpp
	g++ -c intMinHeap.cpp

clean:
	rm -f p2 *.o *~
