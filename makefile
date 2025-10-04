CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread -Iinclude
SRC = src/main.c src/maze.c src/player.c src/enemy.c src/utils.c src/timer.c src/game.c src/render.c src/input.c src/hint.c src/path.c src/matrix.c
OBJ = $(SRC:src/%.c=build/%.o)
BIN = bin/maze-game
ARGS ?=

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