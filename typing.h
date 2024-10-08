#ifndef TYPING_H
#define TYPING_H

#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <time.h>
#include "menu.h"

#define WORD_SIZE 128
#define WORD_COUNT 3000

#define LINE_PADDING 10
#define LINE_COUNT 2

#define REFRESH_MS 1000

int main(int argc, char* argv[]);
void init_curses();
void end_curses();

void run();
void run_loop(char* words[]);
void print_lines(char* lines[], int line_count);
char* get_random_word(char* words[]);
char* gen_random_line(char* words[]);
int get_line_length(int padding);
int get_scrw();

void read_file(char* dest[], char* filename);

#endif
