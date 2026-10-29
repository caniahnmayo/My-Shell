# CMSC-520: Operating Systems
# Project 1: A My Shell Program
# Professor: Dr. Lee
# File Created: 2026.10.17
# File Updated: 2026.10.17

SOURCES = myshell.c                       # Source files for project
OBJECTS = myshell.o                       # Object files for project
CC = gcc                                  # Compiler for project: -Wall
CFLAGS = -Wall                         

project: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o myshell              # Output file

clean: 
	rm *.o myshell                                     # Clean command
