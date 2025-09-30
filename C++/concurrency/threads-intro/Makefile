FLAGS = -g -O0 -Wall -pthread

INCLUDES = ../include 

all: first 

clean:
	rm -f first

first: first.cc 
	g++ -I $(INCLUDES) -o $@ $@.cc $(FLAGS) 