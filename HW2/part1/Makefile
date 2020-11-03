CXX := gcc
CXXFLAGS := -I./common -O3 -mavx2 -std=gnu99 -Wall


all: pi.out

vectorOP.o: vectorOP.cpp vectorOP.h
	$(CXX) $(CXXFLAGS) -c vectorOP.cpp
serialOP.o: serialOP.cpp serialOP.h
	$(CXX) $(CXXFLAGS) -c serialOP.cpp

pi.out: main.cpp 
	$(CXX) $(CXXFLAGS) main.cpp -o pi.out -lpthread

clean:
	rm -f *.o *.s *.out *~
