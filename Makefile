CXX = g++
CXXFLAGS = -O2 -o $@

all: arc beladi

arc: ./ARC/main.cpp ./ARC/arc.hpp
	$(CXX) $(CXXFLAGS) $<

beladi: ./Beladi/main.cpp ./Beladi/beladi.hpp
	$(CXX) $(CXXFLAGS) $<
