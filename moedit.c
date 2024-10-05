#include <stdio.h>
#include <ncurses.h>

int main_window_y = 0, main_window_x = 0;
WINDOW *main_window;

#define ESC_KEY 27
#define COLON_KEY 58
#define i_KEY 105
#define j_KEY 106
#define k_KEY 107
#define l_KEY 108
#define h_KEY 104

void window_initialisation();
void initiate_reading_printing(char *file_name);
void mode_handling();
void normal_mode(int key_pressed, int cursor_y, int cursor_x);
void command_mode(int key_pressed);
void command_mode_parser(char *command_input);

int main(int argc, char* argv[])
{
	if (argc > 2)
	{
		printf("fucker you cant enter more than 1 argument, so do ./editor filename.txt\n");
		printf("you entered %d arguments\n", argc);
		endwin();
	}
	else
	{
		window_initialisation();
		initiate_reading_printing(argv[1]);
		mode_handling();
		endwin();
	}

	return 0;
}

void window_initialisation()
{
	initscr();
	noecho();
	cbreak();
	//getyx(stdscr, main_window_y, main_window_x);
	main_window = newwin(main_window_y, main_window_x, 0, 0);
	if (main_window == NULL)
	{
		endwin();
		printf("There was an error initialising the window, no fucking clue why :3\n");
	}
	refresh();
	box(main_window, main_window_y, main_window_x);
}

void initiate_reading_printing(char *file_name)
{
	FILE *file = fopen(file_name, "r");
	if (file == NULL)
	{
		printf("Error opening the %s, could be that the file doesnt exist\n", file_name);
	}
	char buffer_for_text[200];
	int line = 0, row = 0;
	while (fgets(buffer_for_text, sizeof(buffer_for_text), file))
	{
		mvwprintw(main_window, line++, row++, "%s", buffer_for_text);
	}

	wrefresh(main_window);
}

void mode_handling()
{
	int editor_is_active = 1;
	int key_pressed;
	int cursor_y, cursor_x;
	getyx(stdscr, cursor_y, cursor_x);
	while (editor_is_active == 1)
	{
		normal_mode(key_pressed, cursor_y, cursor_x);
		command_mode(key_pressed);
	}
}

void normal_mode(int key_pressed, int cursor_y, int cursor_x)
{
	while (key_pressed != i_KEY || key_pressed != COLON_KEY)
	{
		switch (key_pressed = getch())
		{
			case j_KEY:

			cursor_y++;
			move(cursor_y, cursor_x);

			break;

			case k_KEY:

			cursor_y--;
			move(cursor_y, cursor_x);

			break;

			case l_KEY:

			cursor_x++;
			move(cursor_y, cursor_x);

			break;

			case h_KEY:

			cursor_x--;
			move(cursor_y, cursor_x);

			break;
		}
	}
}

void insert_mode()
{

}

void command_mode(int key_pressed)
{
	if (key_pressed == COLON_KEY)
	{
		char buffer_for_command[10];
		fgets(buffer_for_command, sizeof(buffer_for_command), stdin); 
		command_mode_parser(buffer_for_command);
	}
}

void command_mode_parser(char *command_input)
{

}


