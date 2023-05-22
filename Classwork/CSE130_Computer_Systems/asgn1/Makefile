#Anthony White
#CSE 130 Assignment 0
#Makefile

CC = clang

# compiler flags:
CFLAGS  = -Wall -Werror -Wextra -pedantic 
  
# the name to use for both the target source file, and the output file:
TARGET = httpserver
  
all: $(TARGET)
  
$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean: 
	$(RM) $(TARGET)
