CXX := gcc
CXXFLAGS := -I./common -O3 -mavx2 -std=gnu99 -Wall 
HEADERS=include/simdxorshift128plus.h  include/xorshift128plus.h  

OBJECTS= simdxorshift128plus.o xorshift128plus.o    

all: pi.out $(OBJECTS)

xorshift128plus.o: ./src/xorshift128plus.c $(HEADERS)
	$(CXX) $(CXXFLAGS) -c ./src/xorshift128plus.c -Iinclude
   
simdxorshift128plus.o: ./src/simdxorshift128plus.c $(HEADERS)
	$(CXX) $(CXXFLAGS) -c ./src/simdxorshift128plus.c -Iinclude 


pi.out: main.c $(HEADERS) $(OBJECTS) 
	$(CXX) $(CXXFLAGS) main.c  $(OBJECTS) -o pi.out -Iinclude -flto -lpthread 


clean:
	rm -f *.o *.s *.out *~
