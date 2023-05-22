#Anthony White
#CSE 130 Assignment 0
#Makefile

CC = clang

# compiler flags:
CFLAGS  = -pthread -Wall -Werror -Wextra -pedantic 
  
# the name to use for both the target source file, and the output file:
TARGET = httpserver
  
all: $(TARGET)
  
$(TARGET): $(TARGET).o my_queue.o
	$(CC) $(CFLAGS) $(TARGET).o my_queue.o -o $(TARGET)

$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) -c $(TARGET).c

my_queue.o: my_queue.c my_queue.h
	$(CC) $(CFLAGS) -c my_queue.c

clean: 
	$(RM) $(TARGET) *.o *~