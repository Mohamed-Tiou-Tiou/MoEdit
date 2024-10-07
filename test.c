#include <stdio.h>
#include <ncurses.h>
struct Window
{
    int x = 0;
    int y = 0;
};

struct Window *create_new_window(char *name, int x_width, int y_height)
{
	
    WINDOW *name = newwin(x_width, y_height, 0, 0);
    Window->x = x_width;
    Window->y = y_width;
    return name;
}

int main()
{
	create_new_window(main_window, 10, 10);
	return 0;
}

