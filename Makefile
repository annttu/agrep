GPP = g++
LDFLAGS = -lm
CXXFLAGS = -g -funroll-all-loops -O3

project: 
	$(GPP) agrep.cpp $(CXXFLAGS) $(LDFLAGS) -o agrep

strip:
	strip agrep

all: project

clean:
	rm -f $(SOURCES) agrep
