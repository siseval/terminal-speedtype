#include "typing.h"
#include "menu.h"
#include <curses.h>

int main(int argc, char* argv[])
{
    init_curses();
    srand(time(NULL));

    run();
    return 0;
}

void run()
{
    char* words[WORD_COUNT];
    read_file(words, "../words.csv");

    main_menu(words);
}
void quit()
{
    end_curses();
    exit(0);
}

void main_menu(char* words[])
{
    struct button b_fifteen_sec = {"15 sec"};
    struct button b_thirty_sec = {"30 sec"};
    struct button b_sixty_sec = {"60 sec"};
    struct button b_quit = {"Quit"};

    struct menu time_menu = {"=: Time limit :=", ":: ", " ::", 3, 1, true, false, 4, 0, b_fifteen_sec, b_thirty_sec, b_sixty_sec, b_quit};
    int gaps[] = {4, 2, 2, 4};

    int selection = do_menu(&time_menu, gaps, 0, true);

    int times[] = {15, 30, 60};

    if (selection == 3)
    {
        free_words(words);
        quit();
    }

    main_loop(words, times[time_menu.selected]);
}

void end_menu(char* words[], int num_correct, int num_typed, int seconds)
{
    clear();

    struct button b_retry = {"Retry"};
    struct button b_to_menu = {"Main Menu"};

    struct menu end_menu = {"", ":: ", " ::", 3, 1, true, false, 2, 0, b_retry.text, b_to_menu};
    int gaps[] = {2, 2};

    print_stats(num_correct, num_typed, seconds);

    do_menu(&end_menu, gaps, 4, false); 

    end_menu.selected ? main_menu(words) : main_loop(words, seconds);
}

void print_stats(int num_correct, int num_typed, int seconds)
{
    int accuracy_percent = calc_accuracy(num_correct, num_typed);
    char accuracy_str[20];
    sprintf(accuracy_str, "Accuracy: %d%%", accuracy_percent);

    float wpm = calc_wpm(num_correct, seconds);
    char wpm_str[20];
    sprintf(wpm_str, "WPM: %.0f", wpm);

    struct button l_accuracy = {accuracy_str};
    struct button l_wpm = {wpm_str};

    struct menu stats = {"=: Stats :=", "- ", " -", 1, 1, true, false, 2, 0, l_accuracy, l_wpm}; 
    int gaps[] = {4, 2};

    print_as_labels(stats, gaps, -4, false);
}

void main_loop(char* words[], int time_limit_sec)
{
    char* lines[LINE_COUNT];
    char* typed[LINE_COUNT - 1]; 
    allocate_strings(lines, typed, words);

    bool* is_correct = malloc(sizeof(bool[MAX_TYPED]));
    int num_typed = 0;

    bool is_rotated = false;
    time_t t0 = time(NULL);

    print_lines(lines, typed, is_rotated);

    while (time(NULL) - t0 < time_limit_sec)
    {
        time_t seconds = time(NULL) - t0;
        print_top(seconds);

        char input = handle_input(typed[is_rotated], lines[is_rotated], is_correct, &num_typed);
        is_rotated = handle_rotation(lines, typed, words, is_rotated, input);
        
        print_lines(lines, typed, is_rotated);
    }

    free_strings(lines, typed);
    free(is_correct);

    end_menu(words, num_correct(is_correct, &num_typed), num_typed, time_limit_sec);
}

void allocate_strings(char* lines[], char* typed[], char* words[])
{
    for (int i = 0; i < LINE_COUNT; i++)
    {
        lines[i] = gen_random_line(words);
    }
    for (int i = 0; i < LINE_COUNT - 1; i++)
    {
        typed[i] = malloc(sizeof(char[get_line_length(0)]));
        empty_string(typed[i]);
    }
}

void free_strings(char* lines[], char* typed[])
{
    for (int i = 0; i < LINE_COUNT; i++)
    {
        free(lines[i]);
    }
    for (int i = 0; i < LINE_COUNT - 1; i++)
    {
        free(typed[i]);
    }
}

void free_words(char* words[])
{
    for (int i = 0; i < WORD_COUNT; i++)
    {
        free(words[i]);
    }
}

void empty_string(char* string)
{
    for (int i = 0; i < get_line_length(0); i++)
    {
        string[i] = '\0';
    }
}

char handle_input(char* typed, char* line, bool* is_correct, int* num_typed)
{
    char input = getch();
    input = input == K_ENTER ? ' ' : input;
    switch (input)
    {
        case ERR:
            break;
        case K_BACKSPACE:
            if (strlen(typed) < 1) { input = EMPTY; }
            is_correct[*num_typed] = false;
            *num_typed -= 1;
            typed[strlen(typed) - 1] = '\0';
            break;
        default:
            typed[strlen(typed)] = input;
            is_correct[*num_typed] = typed[strlen(typed) - 1] == line[strlen(typed) - 1];
            *num_typed += 1;
            break;
    }
    printw("%d", num_typed);
    return input;
}

bool handle_rotation(char* lines[], char* typed[], char* words[], bool is_rotated, char input)
{
    if (should_rotate(lines, typed[is_rotated], input, is_rotated))
        {
            rotate_lines(lines, words, is_rotated);
            if (is_rotated)
            {
                strcpy(typed[0], typed[1]);
            }
            is_rotated = true;

            empty_string(typed[is_rotated]);
        }
    if (is_rotated && input == EMPTY && typed[1][0] == '\0')
        {
            is_rotated = false;
            typed[0][strlen(typed[0]) - 1] = '\0';
            empty_string(typed[1]);
        }
    return is_rotated;
}

bool should_rotate(char* lines[], char* typed, char input, bool has_rotated)
{
    return input != ERR && strlen(typed) >= strlen(lines[has_rotated]);
}
void rotate_lines(char* lines[], char* words[], bool has_rotated)
{
    if (has_rotated)
    {
        strcpy(lines[0], lines[1]);
        strcpy(lines[1], lines[2]);
        strcpy(lines[2], gen_random_line(words));
    }
}

void print_top(time_t seconds)
{
    attron(A_BOLD);
    move_center_v(-6);
    center_string(num_length(seconds)); 
    printw("%ld", seconds);
    attroff(A_BOLD);
}

int num_correct(bool* is_correct, int* num_typed)
{
    int num_correct = 0;
    for (int i = 0; i < *num_typed; i++)
    {
        num_correct += is_correct[i];
    }
    return num_correct;
}

int calc_accuracy(int num_correct, int num_typed)
{
    return num_correct * 100 / num_typed;
}

float calc_wpm(int num_correct, int seconds)
{
    return ((float)num_correct / ((float)seconds / 60)) / 4;
}

int num_length(int value) 
{
    int l = 1;
    while (value > 9) 
    {
        l++;
        value /= 10;
    }
    return l;
}

void prepare_print()
{
    clear();
    move_center_v(-3);
    attron(COLOR_PAIR(1));
}

void print_lines(char* lines[], char* typed[], int cur_line)
{
    prepare_print();
    for (int i = 0; i < LINE_COUNT; i++)
    {
        print_line(lines[i], i <= cur_line ? typed[i] : NULL);
    }
}

void print_line(char* line, char* typed)
{
    center_string(strlen(line));
    for (int c = 0; c < strlen(line); c++)
    {
        handle_color(line, typed, c);
        bool do_underscore = false;
        if (typed != NULL && line[c] == ' ')
        {
            do_underscore = (typed[c] != ' ' && typed[c] != '\0') || strlen(typed) == c;
        }
        do_underscore ? printw("_") : printw("%c", line[c]);
    }
    printw("\n\n\r");
}

void handle_color(char* line, char* typed, int c)
{
    if (typed == NULL) 
    {
        attroff(A_BOLD);
        attron(COLOR_PAIR(1));
        return;
    }
    if (strlen(typed) == c)
    {
        attron(A_BOLD);
        attron(COLOR_PAIR(3));
        return;
    }
    if (typed[c] == '\0')
    {
        attroff(A_BOLD);
        attron(COLOR_PAIR(1));
        return;
    }
    if (line[c] == typed[c])
    {
        attron(A_BOLD);
        attron(COLOR_PAIR(1));
        return;
    }
    attron(A_BOLD);
    attron(COLOR_PAIR(2));
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

void center_string(int length)
{
    move_center_h(-((int)(length / 2)));
}

int get_line_length(int padding)
{
    return get_scrw() - padding * 2;
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
    init_pair(2, COLOR_RED, -1);
    init_pair(3, COLOR_YELLOW, -1);
    init_pair(4, COLOR_GREEN, -1);
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
