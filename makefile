CC = g++
CFLAGS = -std=c++2a -Ox

compare_cells: main.o disagreements.o cell.o
	$(CC) $(CFLAGS) -o compare_cells main.o disagreements.o cell.o

main.o: main.cpp disagreements.o cell.o

disagreements.o: disagreements.hpp disagreements.cpp cell.o

cell.o: cell.cpp cell.hpp