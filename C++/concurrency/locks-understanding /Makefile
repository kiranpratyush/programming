FLAGS = -g -O0 -Wall -pthread

INCLUDES = ../include 

all: lock wrong-load-set

clean:
	rm -f lock

lock: lock.cc 
	g++ -I $(INCLUDES) -o $@ $@.cc $(FLAGS)
wrong-load-set: wrong-load-set.cc
	g++ -I $(INCLUDES) -o $@ $@.cc $(FLAGS)
test-set-atomic-spinlock: test-set-atomic-spinlock.c
	gcc -I $(INCLUDES) -o $@ $@.c $(FLAGS)