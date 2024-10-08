#include "typing.h"
#include <curses.h>

int main(int argc, char* argv[])
{
    init_curses();
    srand(time(NULL));

    run();

    end_curses();

    return 0;
}

void run()
{
    char* words[WORD_COUNT];
    read_file(words, "../words.csv");

    run_loop(words);
}

void run_loop(char* words[])
{
    char* lines[LINE_COUNT];
    lines[0] = gen_random_line(words);
    lines[1] = gen_random_line(words);

    while (true)
    {
        getch();
        print_lines(lines, LINE_COUNT);
    }
}

void print_lines(char* lines[], int line_count)
{
    clear();
    attron(COLOR_PAIR(1));
    for (int l = 0; l < line_count; l++)
    {
        to_center(strlen(lines[l]), get_scrw());
        for (int c = 0; c < strlen(lines[l]); c++)
        {
            printw("%c", lines[l][c]);
        }
        printw("\n\n\r");
    }
}

char* get_random_word(char* words[])
{
    return words[rand() % WORD_COUNT];
}

char* gen_random_line(char* words[])
{
    int line_length = get_line_length(LINE_PADDING);
    char* line = malloc(sizeof(char[line_length]));
    while (strlen(line) < line_length)
    {
        strcat(line, get_random_word(words));
        strcat(line, " ");
    }
    return line;
}

int get_line_length(int padding)
{
    return get_scrw() - padding * 2;
}
int get_scrw()
{
    int scrw, scrh;
    getmaxyx(stdscr, scrh, scrw);
    return scrw;
}


void init_curses()
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    timeout(REFRESH_MS);

    start_color();
    use_default_colors();
    init_pair(1, COLOR_WHITE, -1);
}
void end_curses()
{
    endwin();
}

void read_file(char* dest[], char* filename)
{
    FILE* file_ptr = fopen(filename, "r");
    if (file_ptr == NULL)
    {
        fprintf(stderr, "Could not open file %s\n", filename);
        exit(1);
    }

    for (int i = 0; i < WORD_COUNT; i++)
    {
        dest[i] = malloc(WORD_SIZE);
        fscanf(file_ptr, "%127s", dest[i]);
    }
    fclose(file_ptr);
}
