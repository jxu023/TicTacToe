CFLAGS=-std=c++17
CC=g++

a: main.cpp TicTacToe.o
	$(CC) $(CFLAGS) main.cpp TicTacToe.o && ./a.out

TicTacToe.o: TicTacToe.h TicTacToe.cpp
	$(CC) $(CFLAGS) -c TicTacToe.cpp

.PHONY clean:
	rm -rf a.out TicTacToe.o
