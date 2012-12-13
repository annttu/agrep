GPP = g++
LDFLAGS = -lm
CXXFLAGS = -g

project: 
	$(GPP) agrep.cpp $(CXXFLAGS) $(LDFLAGS) -o agrep

all: project

clean:
	rm -f $(SOURCES) agrep
