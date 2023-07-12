CXXFLAGS = -Wall -lsfml-graphics -lsfml-window -lsfml-system
DEPS = fonctions.h chlamy.h
OBJ = main.o fonctions.o chlamy.o

execute: $(OBJ)
	g++ -o $@ $^ $(CXXFLAGS)
	./execute

%.o: %.cpp $(DEPS)
	g++ -c -o $@ $< $(CXXFLAGS)
