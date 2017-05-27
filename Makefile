CC=g++
OPT=-std=c++11 -Wall -O1
EXEC=pentago

.PHONY: all re clean

all: pentago

pentago:
	$(CC) $(OPT) -Iincludes/ game.cpp ai.cpp gui.cpp -o $(EXEC) -lGL -lglut -fopenmp

clean:
	rm -rf *.o *.exe $(EXEC)  2>/dev/null

re:
	make clean ; make all

