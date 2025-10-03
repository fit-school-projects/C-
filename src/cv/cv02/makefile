CXX = g++
CXXFLAGS = -g -std=c++17 -Wall -pedantic
PROGRAM = erouska

PROGRAM: $(PROGRAM).cpp
	$(info Compiling $(PROGRAM))
	$(CXX) $(CXXFLAGS) $(PROGRAM).cpp -o $(PROGRAM)
	$(info Testing $(PROGRAM))
	./$(PROGRAM)
	rm $(PROGRAM)