#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define ESC_KEY 27
#define COLON_KEY 58
#define ENTER_KEY 10
#define BACK_SPACE_KEY 127
#define p_KEY 112
#define i_KEY 105
#define j_KEY 106
#define k_KEY 107
#define l_KEY 108
#define h_KEY 104

void windows_handling();
void draw_main_window();
void draw_explorer_window();
void draw_description_window();
void draw_status_window();
void draw_command_window();
//void initiate_reading_printing(char * file_name);
void mode_handling();
void normal_mode();
void insert_mode();
void insert_characters_linkedlist();
void delete_one_character();
void print_refresh_list();
void command_mode();
void command_mode_parser(char * command_input);
void check_what_mode_im_in();
void check_if_cursor_at_border_insert_mode();

int cursor_y = 1;
int cursor_x = 1;

int editor_is_active = 1;
int normal_mode_is_active = 1;
int insert_mode_is_active = 0;
int command_mode_is_active = 0;
int explorer_mode_is_active = 0;

int itteration = 0;
int key_pressed;

int rows;
int coloms;

WINDOW * main_window;
int main_window_y;
int main_window_x;

WINDOW * explorer_window;
int explorer_window_y;
int explorer_window_x;

WINDOW * description_window;
int description_window_y;
int description_window_x;

WINDOW * status_window;
int status_window_y;
int status_window_x;

WINDOW * command_window;
int command_window_y;
int command_window_x;

WINDOW * warning_window;
int warning_window_y;
int warning_window_x;

typedef struct node
{
	char character;

	struct node * prev;
	struct node * next;

}node;

node * head = NULL;
node * tail = NULL; 
node * new_node = NULL;

node * create_node(int new_character_ascii)
{
	node * new_node = (node * )malloc(sizeof(node));

	if (new_node == NULL)
	{
		printf("Memory allocation of the new node failed!\n");
		endwin();
	}

	new_node->character = (char)new_character_ascii;
	new_node->prev = NULL;
	new_node->next = NULL;

	return new_node;
}


int main(int argc, char * argv[])
{
	if (argc > 2)
	{
		printf("You cant enter more than 1 argument, so do ./editor filename.txt\n");
		printf("You entered %d arguments\n", argc);
	}
	else
	{
		windows_handling();
		//initiate_reading_printing(argv[1]);
		mode_handling();
		delwin(main_window);
		delwin(status_window);
		delwin(command_window);
		delwin(description_window);
		delwin(explorer_window);
		endwin();
	}

	return 0;
}

void windows_handling()
{
	initscr();
	noecho();
	cbreak();

	rows = 0;
	coloms = 0;
	
	getmaxyx(stdscr, rows, coloms);
	
	draw_explorer_window();
	draw_description_window();
	draw_main_window();
	draw_status_window();
	draw_command_window();

}

void draw_main_window()
{
	main_window_y = rows - 3;
	main_window_x = coloms - (coloms / 5);
	main_window = newwin(main_window_y, main_window_x, 0, coloms / 5);
	if (main_window == NULL)
	{
		endwin();
		printf("There was an error initialising the main window. Try to execute it again\n");
	}
	box(main_window, 0, 0);
	wrefresh(stdscr);
	wrefresh(main_window);
	mvwprintw(main_window, 0, 1, " Main Window ");

}

void draw_explorer_window()
{
	explorer_window_y = (rows - 3) / 2 + 1;
	explorer_window_x = coloms / 5;

	explorer_window = newwin(explorer_window_y, explorer_window_x, 0, 0);

	if (explorer_window == NULL)
	{
		endwin();
		printf("There was an error initialising the explorer window. Try to execute it again\n");
	}

	box(explorer_window, 0, 0);
    mvwprintw(explorer_window, 0, 1, " Explorer Window ");
	wrefresh(stdscr);
	wrefresh(explorer_window);

}

void draw_description_window()
{
	description_window_y = (rows - 3) / 2 - 1;
	description_window_x = coloms / 5;

	description_window = newwin(description_window_y, description_window_x, (rows - 3)  - (rows - 3) / 2 + 1, 0);

	if (description_window == NULL)
	{
		endwin();
		printf("There was an error initialising the description window. Try to execute it again\n");
	}

	box(description_window, 0, 0);
    mvwprintw(description_window, 0, 1, " Description Window ");
	wrefresh(stdscr);
	wrefresh(description_window);
}

void draw_status_window()
{
	status_window_y = 3;
	status_window_x = coloms / 2;
	status_window = newwin(status_window_y, status_window_x, rows - 3, 0);
	if (status_window == NULL)
	{
		endwin();
		printf("There was an error initialising the status window. Try to execute it again\n");
	}
	box(status_window, 0, 0);
    mvwprintw(status_window, 0, 1, " Status Window ");
	wrefresh(stdscr);
	wrefresh(status_window);

}

void draw_command_window()
{
	command_window_y = 3;
	command_window_x = coloms / 2;
	command_window = newwin(command_window_y, command_window_x, rows - 3, coloms / 2);
	if (command_window == NULL)
	{
		endwin();
		printf("There was an error initialising the command window. Try to execute it again\n");
	}
	box(command_window, 0, 0);
    mvwprintw(command_window, 0, 1, " Command Window ");
	wrefresh(stdscr);
	wrefresh(command_window);
}

/*void initiate_reading_printing(char * file_name)
{
	FILE * file = fopen(file_name, "r");
	if (file == NULL)
	{
		printf("Error opening the %s, could be that the file doesnt exist\n", file_name);
	}


	wrefresh(main_window);
}*/

void mode_handling()
{ 
	wmove(main_window, cursor_y, cursor_x);
	wrefresh(main_window);
	while (editor_is_active == 1)
	{
		normal_mode();
		insert_mode();
		command_mode();
	}
}

void normal_mode()
{
	if (normal_mode_is_active == 1)
	{
		check_what_mode_im_in();
	}

	while (normal_mode_is_active == 1)
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

			case i_KEY:

				normal_mode_is_active = 0;
				insert_mode_is_active = 1;
				command_mode_is_active = 0;

			break;

			case COLON_KEY:

				normal_mode_is_active = 0;
				insert_mode_is_active = 0;
				command_mode_is_active = 1;

			break;
			
			case p_KEY:
				
				print_refresh_list();

			break;
		}
	}
}

void insert_mode()
{
	if (insert_mode_is_active == 1)
	{
		check_what_mode_im_in();
	}	

	while (insert_mode_is_active == 1)
	{
		switch (key_pressed = getch())
		{
			case ESC_KEY:
			
				normal_mode_is_active = 1;
				insert_mode_is_active = 0;
				command_mode_is_active = 0;

			break;

			case ENTER_KEY:

				cursor_y++;
				cursor_x = 1;
				wmove(main_window, cursor_y, cursor_x);
				wrefresh(main_window);

			break;

			case BACK_SPACE_KEY: //delete key

				delete_one_character();
				
			break;

			default:

				insert_characters_linkedlist();
		}
	}
}


void insert_characters_linkedlist()
{
	new_node = create_node(key_pressed);

	if (head == NULL)
	{
		head = new_node;
		tail = new_node;
		
		head->prev = NULL;
		head->next = NULL;
	}
	else
	{
		if (head->next == NULL || tail->prev == NULL)
		{
			head->next = new_node;
			tail->prev = new_node;	
		}

		tail->next = new_node;
		new_node->prev = tail;
		tail = new_node;
	}
}

void delete_one_character()
{
	if (head == tail)
	{
		head = NULL;
		tail = NULL;
	}
	else if (head->next != tail->prev)
	{
		node * temp_node = tail;
		tail = tail->prev;
		free(temp_node);
		temp_node = NULL;
		tail->next = NULL;
	}
	else if (head->next == tail->prev)
	{
		node * temp_node = tail;
		tail = tail->prev;
		free(temp_node);
		temp_node = NULL;
		tail->next = NULL;
	}
}

void print_refresh_list()
{
	node * current_node = head;
	while(current_node != NULL)
	{
		check_if_cursor_at_border_insert_mode();
		mvwprintw(main_window, cursor_y, cursor_x, "%c", current_node->character);
		wrefresh(main_window);
		current_node = current_node->next;
		cursor_x++;
	}

	free(current_node);
	current_node = NULL;
}

void command_mode()
{
	if (command_mode_is_active == 1)
	{
		check_what_mode_im_in();
	}
	else
	{
		normal_mode_is_active = 1;
	}

	while (command_mode_is_active == 1)
	{
		char buffer_for_command[10];
		fgets(buffer_for_command, sizeof(buffer_for_command), stdin);
		command_mode_parser(buffer_for_command);
		command_mode_is_active = 0;
		normal_mode_is_active = 1;
	}
	
}

void command_mode_parser(char *command_input)
{

}

void check_what_mode_im_in()
{
	if (normal_mode_is_active == 1)
	{
		mvwprintw(status_window, 1, 1, " NORMAL MODE IS ACTIVE ");
		wrefresh(status_window);
		wrefresh(main_window);
	}
	else if (insert_mode_is_active == 1)
	{
		mvwprintw(status_window, 1, 1, " INSERT MODE IS ACTIVE ");
		wrefresh(status_window);
		wrefresh(main_window);
	}
	else if (command_mode_is_active == 1)
	{
		mvwprintw(status_window, 1, 1, " COMMAND MODE IS ACTIVE ");
		wrefresh(status_window);
		wrefresh(main_window);
	}
	else if (explorer_mode_is_active == 1)
	{
		mvwprintw(status_window, 1, 1, " EXPLORER MODE IS ACTIVE ");
		wrefresh(status_window);
		wrefresh(main_window);
	}
}

void check_if_cursor_at_border_insert_mode()
{
	if (cursor_x > main_window_x - 2)
	{
		cursor_y++;
		cursor_x = 1;
	}
}
