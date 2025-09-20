CC = gcc
CFLAGS = -Wall -Wextra -pthread -Iinclude
SRC = src/main.c src/maze.c src/player.c src/enemy.c src/timer.c src/utils.c
OBJ = $(SRC:src/%.c=build/%.o)
BIN = bin/maze-game

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

build/%.o: src/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build/* bin/*