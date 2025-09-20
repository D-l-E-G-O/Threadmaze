#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>


void clear_screen(void);
int random_int(int min, int max);
char get_input_non_blocking(void);