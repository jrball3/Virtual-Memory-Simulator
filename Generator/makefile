p = generate_input
c = g++
f = -g -std=c++0x

all: Main.o
	$c $f -o $p Main.o

Main.o: Main.cpp
	$c $f -c Main.cpp

clean:
	rm *.o $p
