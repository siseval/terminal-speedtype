#include "menu.h"

void draw_button(struct button b, struct menu m, int col, int scrw) 
{
    attron(COLOR_PAIR(col));
  
    to_center(strlen(b.text) + strlen(m.left) + strlen(m.right), scrw);
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

void to_center(int len, int scrw) 
{
    int fwidth = (scrw / 2) - len / 2;
    char empty[100] = "";
    for (int i = 0; i < fwidth; i++) 
    {
        strcat(empty, " ");
    }
    printw("%s", empty);
}
