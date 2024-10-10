#ifndef TYPING_H
#define TYPING_H

#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <time.h>
#include "menu.h"

#define WORD_SIZE 128
#define WORD_COUNT 3000

#define MAX_TYPED 500

#define LINE_PADDING 10
#define LINE_COUNT 3

#define K_ENTER 10
#define K_BACKSPACE 127

#define EMPTY '\e'

#define REFRESH_MS 1000

int main(int argc, char* argv[]);
void init_curses();
void end_curses();

void run();
void main_menu(char* words[]);
void main_loop(char* words[], int time_limit);

void allocate_strings(char* lines[], char* typed[], char* words[]);
void free_strings(char* lines[], char* typed[]);
void empty_string(char* string);
char handle_input(char* typed, char* line, bool* is_correct, int* num_typed);

bool handle_rotation(char* lines[], char* typed[], char* words[], bool is_rotated, char input);
bool should_rotate(char* lines[], char* typed, char input, bool is_rotated);
void rotate_lines(char* lines[], char* words[], bool is_rotated);

int num_correct(bool* is_correct, int* num_typed);
void print_top(time_t seconds);
int num_length(int value);

void prepare_print();
void handle_lines(char* lines[], char* typed[], char* words[], bool is_rotated);
void print_lines(char* lines[], char* typed[], int cur_line);
void print_line(char* line, char* typed);
void handle_color(char* line, char* typed, int c);

char* get_random_word(char* words[]);
char* gen_random_line(char* words[]);

int get_line_length(int padding);
void move_center_v(int dy);
int get_scrw();
int get_scrh();

void read_file(char* dest[], char* filename);

#endif
