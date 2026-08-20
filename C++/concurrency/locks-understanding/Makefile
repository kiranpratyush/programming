FLAGS = -g -O0 -Wall -pthread

INCLUDES = ../include 

all: lock wrong-load-set test-set-atomic-spinlock compare-and-swap just-yield-with-atomiclock

clean:
	rm -f lock wrong-load-set test-set-atomic-spinlock compare-and-swap just-yield-with-atomiclock

lock: lock.cc 
	g++ -I $(INCLUDES) -o $@ $@.cc $(FLAGS)
wrong-load-set: wrong-load-set.cc
	g++ -I $(INCLUDES) -o $@ $@.cc $(FLAGS)
test-set-atomic-spinlock: test-set-atomic-spinlock.c
	gcc -I $(INCLUDES) -o $@ $@.c $(FLAGS)
compare-and-swap: compare-and-swap.c
	gcc -I $(INCLUDES) -o $@ $@.c $(FLAGS)
just-yield-with-atomiclock: just-yield-with-atomiclock.c
	gcc -I $(INCLUDES) -o $@ $@.c $(FLAGS)