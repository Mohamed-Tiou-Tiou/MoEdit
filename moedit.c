#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>


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

struct Node
{
	char character;

	struct Node *prev;
	struct Node *next;

};

struct Node *create_node(char new_character)
{
	struct Node *new_node = (struct Node*)malloc(sizeof(struct Node));

	if (new_node == NULL)
	{
		printf("Memory allocation of the new node failed!\n");
		endwin();
	}

	new_node->character = new_character;
	new_node->prev = NULL;
	new_node->next = NULL;

	return new_node;
}

WINDOW* main_window;
int main_window_y = 0;
int main_window_x = 0;

int main(int argc, char* argv[])
{
	if (argc > 2)
	{
		printf("You cant enter more than 1 argument, so do ./editor filename.txt\n");
		printf("You entered %d arguments\n", argc);
		endwin();
	}
	else
	{
		window_initialisation();
		initiate_reading_printing(argv[1]);
		mode_handling();
		delwin(main_window);
		endwin();
	}

	return 0;
}

void window_initialisation()
{
	initscr();
	noecho();
	cbreak();

	getmaxyx(stdscr, main_window_y, main_window_x);
	main_window = newwin(main_window_y, main_window_x, 0, 0);

	if (main_window == NULL)
	{
		endwin();
		printf("There was an error initialising the window. Try to execute it again\n");
	}

	box(main_window, 0, 0);
	wrefresh(stdscr);
}

void initiate_reading_printing(char *file_name)
{
	FILE *file = fopen(file_name, "r");
	if (file == NULL)
	{
		printf("Error opening the %s, could be that the file doesnt exist\n", file_name);
	}


	wrefresh(main_window);
}

void mode_handling()
{
	int editor_is_active = 1;
	int key_pressed;
	int cursor_y = 1, cursor_x = 1;

	wmove(main_window, cursor_y, cursor_x);
	wrefresh(main_window);

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

			if (cursor_y < main_window_y - 2)
			{
				cursor_y++;
				wmove(main_window, cursor_y, cursor_x);
				wrefresh(main_window);
			}

			break;

			case k_KEY:
			
			if (cursor_y > 1)
			{
				cursor_y--;
				wmove(main_window, cursor_y, cursor_x);
				wrefresh(main_window);
			}

			break;

			case l_KEY:

			if (cursor_x < main_window_x - 2)
			{
				cursor_x++;
				wmove(main_window, cursor_y, cursor_x);
				wrefresh(main_window);
			}

			break;

			case h_KEY:

			if (cursor_x > 1)
			{
				cursor_x--;
				wmove(main_window, cursor_y, cursor_x);
				wrefresh(main_window);
			}

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


