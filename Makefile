arc: ./ARC/arc.hpp ./ARC/main.cpp
	g++ -O2 ./ARC/arc.hpp ./ARC/main.cpp -o arc

beladi: ./Beladi/beladi.hpp ./Beladi/main.cpp
	g++ -O2 ./Beladi/beladi.hpp ./Beladi/main.cpp -o beladi

all: arc beladi