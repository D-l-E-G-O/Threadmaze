CC = gcc
CFLAGS = -Wall -Wextra -pthread -Iinclude
SRC = src/main.c src/maze.c src/player.c src/enemy.c src/timer.c src/utils.c
OBJ = $(SRC:src/%.c=build/%.o)
BIN = bin/maze-game

.PHONY: all clean dirs run

all: dirs $(BIN)

dirs:
	mkdir -p build bin

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	clear
	./$(BIN) $(ARGS)

clean:
	rm -rf build/* bin/*