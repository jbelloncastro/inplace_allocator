CC=g++
CPPFLAGS=-I../src
#DEBUG=-O0 -g3 -fsanitize=address
RELEASE=-O3 -march=native
CXXFLAGS=-std=c++11 $(DEBUG) $(RELEASE)

targets=\
 basic\
 vector\
 set\
 map

all: $(targets) 

clean:
	rm -f $(targets)

