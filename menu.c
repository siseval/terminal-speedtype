#include "menu.h"

void draw_button(struct button b, struct menu m, int col, int scrw) 
{
    attron(COLOR_PAIR(col));
  
    move_center_h(strlen(b.text) + strlen(m.left) + strlen(m.right));
    printw("%s%s%s\n\r", m.left, b.text, m.right);

    attroff(COLOR_PAIR(col));
}

void draw_buttons(struct menu m, int scrw, int gaps[]) 
{
    m.bold ? attron(A_BOLD) : attroff(A_BOLD);

    for (int i = 0; i < m.num_buttons; i++) 
    {
        struct button b = m.buttons[i];
        for (int j = 0; j < gaps[i]; j++) 
        {
            printw("\n");
        }
        draw_button(b, m, m.selected == i ? m.selected_col : m.normal_col, scrw);
    }

    attroff(A_BOLD);
}

void do_menu(struct menu m, int scrw, int scrh, int gaps[])
{
    
}

struct button get_button(struct menu m, int i) { return m.buttons[i]; }

void menu_input(struct menu *m) 
{

    switch (getch()) 
    {
    case 'k':
    case 'w':
        m->selected -= m->selected <= 0 ? 0 : 1;
        break;

    case 'j':
    case 's':
        m->selected += m->selected >= m->num_buttons - 1 ? 0 : 1;
        break;

    case ' ':
    case 'f':
        m->has_selected = true;
        break;
    }
}

int get_cur_x()
{
    int y, x;
    getyx(stdscr, y, x);
    return x;
}
int get_cur_y()
{
    int y, x;
    getyx(stdscr, y, x);
    return y;
}

int get_scrw()
{
    int scrh, scrw;
    getmaxyx(stdscr, scrh, scrw);
    return scrw;
}

int get_scrh()
{
    int scrh, scrw;
    getmaxyx(stdscr, scrh, scrw);
    return scrh;
}

void move_center_v(int dy)
{
    move((int)(get_scrh() / 2) + dy, get_cur_x());
}

void move_center_h(int dx)
{
    move(get_cur_y(), ((get_scrw() / 2) - dx / 2));
}
