CC = g++
CFLAGS = -std=c++2a -Ox

compare_cells: main.o cell.o
	$(CC) $(CFLAGS) -o compare_cells main.o cell.o

main.o: main.cpp cell.o

cell.o: cell.cpp cell.hpp