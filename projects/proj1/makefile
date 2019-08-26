CXX = g++
CXXFLAGS = -Wall

proj1: p1test10.cpp InnerCB.o CBofCB.o
	$(CXX) $(CXXFLAGS) p1test10.cpp InnerCB.o CBofCB.o -o proj1

InnerCB.o: InnerCB.h InnerCB.cpp 
	$(CXX) $(CXXFLAGS) -c InnerCB.cpp

CBofCB.o: CBofCB.h CBofCB.cpp
	$(CXX) $(CXXFLAGS) -c CBofCB.cpp

run: 
	./proj1

val1: 
	valgrind ./proj1 

clean: 
	rm*~