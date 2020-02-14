#include <exception>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <unistd.h>
#include <ncurses.h>

#define DEFAULT_COLOR_PAIR 1
#define MY_ATTRIBUTES A_BOLD    //по умолчанию установить A_NORMAL
#define CLEANER ' '

using namespace std;

class General_exception : public exception
{
private:
	string error_message;
public:
	General_exception(string message)
	{
		error_message = message;
	}
	const char* what() const noexcept override
	{
		return error_message.c_str();
	}
};
/*
class Ncurses_exception : public _General_exception
{
public:
	Ncurses_exception(string message)
	{
		//General_exception A = new General_exception(message);
	}
};
*/
class Random_symbol
{
private:
	char symbol;
	int attribute;
public:
	Random_symbol(bool polychromy) 
	{
		symbol = rand() % 94 + 33;
		if (polychromy)
		{
			attribute = COLOR_PAIR(rand() % 7 + 1);
		}
		else
		{
			attribute = COLOR_PAIR(DEFAULT_COLOR_PAIR);
		}
	}
	
	void print_Random_symbol(int y_poz, int x_poz)
	{
		if (mvaddch(y_poz, x_poz, symbol | attribute | MY_ATTRIBUTES) == ERR)
		{
			throw General_exception("ncurses: mvaddch error");
		}
	}
};

class Line
{
private:
	int line_size;
	int x_coord;
	int y_top_coord;
	int y_bottom_coord;
	int y_bottom_window_poz;
	bool polychromy;
public:
	Line(int size, int x_poz, int y_bottom_win, bool l_polychromy)
	{
		line_size = size;
		x_coord = x_poz;
		y_bottom_coord = 0;
		y_top_coord = 0;
		y_bottom_window_poz = y_bottom_win;
		polychromy = l_polychromy;
	}

	//вернет: ползет ли линия
	bool print_line_iter() 
	{
		if ((y_top_coord - 1) >= y_bottom_window_poz)
		{
			if (y_bottom_coord != y_top_coord)
			{
				mvaddch(y_bottom_coord, x_coord, CLEANER);
				y_bottom_coord++;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if ((y_top_coord - y_bottom_coord) >= line_size)
			{
				mvaddch(y_bottom_coord, x_coord, CLEANER);
				y_bottom_coord++;
			}
			Random_symbol* temp = new Random_symbol(polychromy);
			temp->print_Random_symbol (y_top_coord, x_coord);
			delete temp;
			y_top_coord++;
		}
		return true;
	}
};

class Main_controller
{
private:
	int line_size;
	int line_spawn_frequency;
	int symbol_spawn_frequency;
	bool polychromy;
public:
	Main_controller() 
	{
		srand(time(NULL));
		line_size = 5;
		line_spawn_frequency = 1;
		symbol_spawn_frequency = 1;
		polychromy = true;
		cout << "Hello!" << endl;
	}
	
	void get_parameters()
	{
		cout << "Enter the following parameters:" << endl;
		cout << "Size of line: ";
		//cin >> line_size;
		cout << "Line spawn frequency: ";
		//cin >> line_spawn_frequency;
		cout << "Symbol spawn frequency: ";
		//cin >> symbol_spawn_frequency;
		cout << "Do I use different colors: ";
		//cin >> polychromy;*/
		getchar();
	}
	
	void ncurses_start(bool if_cursor, bool if_keypad, bool if_echo, bool if_color, int number_of_pairs, int* colors_array)
	{
    	if (!initscr()) 
		{
			cerr << "ncurses initialization error!" << endl;
			exit(EXIT_FAILURE);
   		}
   		if (if_cursor) 
		{
        	curs_set(1);
   		}
   		else 
		{
       		curs_set(0);
    	}
    	if (if_keypad)
		{
        	keypad(stdscr, 1);
    	}
    	else 
		{
        	keypad(stdscr, 0);
    	}
    	if (if_echo) 
		{
        	echo();
    	}
    	else 
		{
        	noecho();
    	}
    	cbreak();    //частичный контроль клавиатуры (Ctrl+С - закроет программу)
    	if (if_color) 
		{
        	start_color();
        	if (!has_colors()) 
			{
            	cerr << "This terminal does not support colors!" << endl;
				endwin();
            	exit(EXIT_FAILURE);
        	}
        	int i = 0, n = 1, check = number_of_pairs;
        	while (number_of_pairs)
			{
            	init_pair(n,colors_array[i],colors_array[i+1]);
            	i += 2;
            	n++;
            	number_of_pairs--;
        	}
        	if (check != 0) 
			{
            	wattrset(stdscr,COLOR_PAIR(1));
        	}
    	}
	}

	void drawing_start()
	{
		int x_max;
		int y_max;
		int lines_array_size = 50;
		getmaxyx(stdscr,y_max,x_max);
		Line** lines_array = new Line* [lines_array_size];
		bool can_allocate_mem = true;
		int lines_array_top_index = 0;
		int lines_array_bottom_index = 0;
		int lines_array_current_index = 0;
		while (1)
		{
			//создание линий
			for (int i = 0; i < line_spawn_frequency; i++)
			{
				lines_array[lines_array_top_index] = new Line(line_size,rand() % x_max,y_max,polychromy);
				lines_array_top_index++;
			}
			//печать линий
			for (int i = 0; i < symbol_spawn_frequency; i++)
			{
				for (int i = 0; i < lines_array_top_index; i++)
				{
					if (lines_array[i]->print_line_iter() == false)
					{
						delete lines_array[i];
						can_allocate_mem = false;
						lines_array_current_index = 0;
					}
				}
			}
			refresh();
			sleep(1);
		}
	}
	
	void ncurses_end()
	{
		endwin();
	}
};

int main(int argc, char* argv[])
{
	int colors_array[] = {COLOR_GREEN,0,COLOR_RED,0,COLOR_BLUE,0,COLOR_CYAN,0,COLOR_YELLOW,0,COLOR_MAGENTA,0,COLOR_WHITE,0};
	Main_controller controller;
	controller.get_parameters();
	controller.ncurses_start(0,1,0,1,7,colors_array);
	controller.drawing_start();
	//гипотетический controller.drawing_stop...
	//controller.ncurses_end();
	//exit(EXIT_SUCCESS);
}
