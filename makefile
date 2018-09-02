COMP = g++-5.3.0
OBJS = jigsawPuzzle.o puzzlePieces.o puzzleElement.o edges.o mainAux.o main.o
EXEC = ex2
CPP_COMP_FLAG = -std=c++14 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) -o $@
#a rule for building a simple c++ source file
#use g++ -MM main.cpp to see dependencies
edges.o: edges.h edges.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
puzzleElement.o: edges.h puzzleElement.h puzzleElement.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
puzzlePieces.o: edges.h puzzleElement.h puzzlePieces.h puzzlePieces.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
jigsawPuzzle.o: puzzlePieces.h jigsawPuzzle.h jigsawPuzzle.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
mainAux.o: jigsawPuzzle.h mainAux.h mainAux.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
main.o: main.cpp mainAux.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
clean:
	rm -f $(OBJS) $(EXEC)