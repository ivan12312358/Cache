CXX = g++
CXXFLAGS = -O2 -o $@

.PHONY: arc beladi

arc: ./ARC/main.cpp
	$(CXX) $(CXXFLAGS) ./ARC/main.cpp

beladi: ./Beladi/main.cpp
	$(CXX) $(CXXFLAGS) ./Beladi/main.cpp

all: arc beladi