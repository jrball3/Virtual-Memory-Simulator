PROJ = program2
FLAGS = -g -std=c++0x 
COMP = g++

all: Driver.o VirtualMemorySim.o
	$(COMP) $(FLAGS) -o $(PROJ) Driver.o VirtualMemorySim.o

Driver.o: Driver.cpp
	$(COMP) $(FLAGS) -c Driver.cpp

VirtualMemorySim.o: VirtualMemorySim.cpp
	$(COMP) $(FLAGS) -c VirtualMemorySim.cpp

clean:
	rm *.o $(PROJ)
