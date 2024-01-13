CXXFLAGS = 

CXXINCLUDES = 

main.exe: main.o
	g++ $(CXXFLAGS) -o main.exe main.o

main.o: main.cpp set.hpp
	g++ $(CXXFLAGS) -I$(CXXINCLUDES) -o main.o -c main.cpp 

.PHONY: clean

clean:
	rm -f *.o *.exe prova.txt

doc:
	doxygen Doxyfile

all: main.exe doc
