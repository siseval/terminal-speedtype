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
#define LINE_COUNT 3

#define K_ENTER 10
#define K_BACKSPACE 127

#define REFRESH_MS 1000

int main(int argc, char* argv[]);
void init_curses();
void end_curses();

void run();
void run_loop(char* words[]);

void empty_string(char* string);
char handle_input(char* typed);

bool should_rotate(char* lines[], char* typed, char input, bool has_rotated);
void rotate_lines(char* lines[], char* words[], bool has_rotated);

void prepare_print();
void print_lines(char* lines[], char* typed, int cur_line);
void print_line(char* line, char* typed);
void handle_color(char* line, char* typed, int c);

char* get_random_word(char* words[]);
char* gen_random_line(char* words[]);

int get_line_length(int padding);
int get_scrw();
int get_scrh();

void read_file(char* dest[], char* filename);

#endif
