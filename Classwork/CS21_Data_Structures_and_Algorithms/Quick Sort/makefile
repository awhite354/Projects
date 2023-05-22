# simple makefile

# define target, it's dependencies and files
p3: main.o
	g++ -o p3 main.o

main.o: main.cpp 
	g++ -c main.cpp

# clean ip
clean:
	rm -f p3 *.o *~
