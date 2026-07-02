FLAGS = -g -O0 -Wall

INCLUDES = ../include 

clean:
	rm -f hello-world dangling-local-variable waiting-for-thread-to-complete using-mutex
	
hello-world: hello-world.cc
	g++ -I $(INCLUDES) -o $@ $@.cc $(FLAGS)

dangling-local-variable: dangling-local-variable.cc
	g++ -I $(INCLUDES) -o $@ $@.cc $(FLAGS)
waiting-for-thread-to-complete: waiting-for-thread-to-complete.cc
	g++ -I $(INCLUDES) $(FLAGS) -o $@ $@.cc 
using-mutex: using-mutex.cc
	g++ -I $(INCLUDES) $(FLAGS) -o $@ $@.cc 