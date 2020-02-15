#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>

#define DEFAULT_POLYCHROMY true
#define DEFAULT_SYMBOL_SPAWN_FREQUENCY 1
#define DEFAULT_LINE_SPAWN_FREQUENCY 1
#define DEFAULT_LINE_SIZE 5
#define DEFAULT_COLOR_PAIR 1
#define DEFAULT_LINES_ARRAY_SIZE 300
#define MY_ATTRIBUTES A_BOLD    //по умолчанию установить A_NORMAL
#define CLEANER ' '

using namespace std;

class GeneralException : public exception
{
private:
	string errorMessage_;
public:
	GeneralException(string message)
	{
		errorMessage_ = message;
	}
	
	const char* what() const noexcept override
	{
		return errorMessage_.c_str();
	}
};

class NcursesException : public GeneralException
{
public:
	NcursesException(string message) : GeneralException(message)
	{}
};

class UserInputException : public GeneralException
{
public:
	UserInputException(string message) : GeneralException(message)
	{}
};

class RandomSymbol
{
private:
	char symbol_;
	int attribute_;
public:
	RandomSymbol(bool isPolychromy) 
	{
		symbol_ = rand() % 94 + 33;
		if (isPolychromy)
		{
			attribute_ = COLOR_PAIR(rand() % 7 + 1);
		}
		else
		{
			attribute_ = COLOR_PAIR(DEFAULT_COLOR_PAIR);
		}
	}
	
	void printRandomSymbol(int yCoordinate, int xCoordinate)
	{
		mvaddch(yCoordinate, xCoordinate, symbol_ | attribute_ | MY_ATTRIBUTES);
	}
};

class Line
{
private:
	int size_;
	int xCoordinate_;
	int yTopCoordinate_;
	int yBottomCoordinate_;
	int yWindowBottomCoordinate_;
	bool isPolychromy_;
public:
	Line(int lineSize, int xCoordinate, int yWindowBottomCoordinate, bool isPolychromy)
	{
		size_ = lineSize;
		xCoordinate_ = xCoordinate;
		yTopCoordinate_ = 0;
		yBottomCoordinate_ = 0;
		yWindowBottomCoordinate_ = yWindowBottomCoordinate;
		isPolychromy_ = isPolychromy;
	}

	void printNextStep() 
	{
		if ((yTopCoordinate_ /*- 1*/) >= yWindowBottomCoordinate_)
		{
			if (yBottomCoordinate_ != yTopCoordinate_)
			{
				mvaddch(yBottomCoordinate_, xCoordinate_, CLEANER);
				yBottomCoordinate_++;
			}
			else
			{
				throw GeneralException("The line completed its task.");
			}
		}
		else
		{
			if ((yTopCoordinate_ - yBottomCoordinate_) >= size_)
			{
				mvaddch(yBottomCoordinate_, xCoordinate_, CLEANER);
				yBottomCoordinate_++;
			}
			RandomSymbol* temp = new RandomSymbol(isPolychromy_);
			temp->printRandomSymbol(yTopCoordinate_, xCoordinate_);
			delete temp;
			yTopCoordinate_++;
		}
	}
};

class MainController
{
private:
	int lineSize_;
	int lineSpawnFrequency_;
	int symbolSpawnFrequency_;
	bool isPolychromy_;

	char* strRealloc_(char* input, int currentSize, int newSize)
	{
		char* output = new char [newSize];
		for (int i = 0; i < currentSize; i++)
		{
			if (i == newSize - 1)
			{
				output[i] = 0;
				break;
			}
			output[i] = input[i];
		}
		delete [] input;
		return output;
	}
	
	Line** lineRealloc_(Line** input, int currentSize, int newSize)
	{
		Line** output = new Line* [newSize];
		for (int i = 0; i < currentSize; i++)
		{
			if (i == newSize)
			{
				break;
			}
			output[i] = input[i];
		}
		delete [] input;
		return output;
	}

	char* readCharStr_(FILE* filestream, int arrayIncrementStep) 
	{
		int asz = 0;
		int c;
		int arraySize = arrayIncrementStep;
		char* str = new char [arraySize];
		*str = 0;
		while (1)
		{
			c = fgetc(filestream);
			if ((c == '\n') || (c == EOF) || (c == '\0'))
			{
		    	return str;
			}
			str[asz++] = c;
			if (asz >= arraySize) 
			{
				str = strRealloc_(str,asz,arraySize + arrayIncrementStep);
				arraySize += arrayIncrementStep;
			}
			str[asz] = 0;
		}
	}
	
public:
	MainController() 
	{
		lineSize_ = DEFAULT_LINE_SIZE;
		isPolychromy_ = DEFAULT_POLYCHROMY;
		lineSpawnFrequency_ = DEFAULT_LINE_SPAWN_FREQUENCY;
		symbolSpawnFrequency_ = DEFAULT_SYMBOL_SPAWN_FREQUENCY;
		srand(time(NULL));
	}

	void readParameters()
	{
		/*char* messages_array[] = {"Enter the following parameters:","Size of line: ","Line spawn frequency: ","Symbol spawn frequency: ","Do I use different colors: "};
		char polychromy_input[4];
		printf("%s",messages_array[0]);
		printf("%s",messages_array[1]);
		printf("%s",messages_array[2]);
		printf("%s",messages_array[3]);*/
	}
	
	void ncursesStart(bool useCursor, bool useKeypad, bool useEcho, bool useColor, int nColorPairs, int* colorsArray)
	{
    	if (!initscr()) 
		{
			throw NcursesException("Initialization error.");
		}
   		if (useCursor) 
		{
        	curs_set(1);
   		}
   		else 
		{
       		curs_set(0);
    	}
    	if (useKeypad)
		{
        	keypad(stdscr, 1);
    	}
    	else 
		{
        	keypad(stdscr, 0);
    	}
    	if (useEcho) 
		{
        	echo();
    	}
    	else 
		{
        	noecho();
    	}
    	cbreak();    //частичный контроль клавиатуры (Ctrl+С - закроет программу)
    	if (useColor) 
		{
        	start_color();
        	if (!has_colors()) 
			{
            	throw NcursesException("This terminal does not support colors.");
        	}
        	int i = 0;
			int pairNo = 1;
			int check = nColorPairs;
        	while (nColorPairs)
			{
            	init_pair(pairNo,colorsArray[i],colorsArray[i+1]);
            	i += 2;
            	pairNo++;
            	nColorPairs--;
        	}
        	if (check != 0) 
			{
            	wattrset(stdscr,COLOR_PAIR(1));
        	}
    	}
	}
	
	void drawingStart()
	{
		int xMaxCoordinate;
		int yMaxCoordinate;
		getmaxyx(stdscr, yMaxCoordinate, xMaxCoordinate);
		bool canAllocateMemory = true;
		int linesArrayTopIndex = 0;
		int linesArrayBottomIndex = 0;
		int linesArrayCurrentIndex = 0;
		int linesArraySize = DEFAULT_LINES_ARRAY_SIZE;
		Line** linesArray = new Line* [linesArraySize];
		struct timespec request;
		request.tv_sec = 0;
		request.tv_nsec = 90000000;
		while (1)
		{
			for (int i = 0; i < lineSpawnFrequency_; i++)
			{
				linesArray[linesArrayTopIndex] = new Line(lineSize_, rand() % xMaxCoordinate, yMaxCoordinate, isPolychromy_);
				linesArrayTopIndex++;
			}
			//символы должны печататься по 1 за заход (изменять можно время)
			//у линий число меняется из расчета чтоб за 1 сек было нужное чисо
			for (int i = 0; i < symbolSpawnFrequency_; i++)
			{
				for (int i = 0; i < linesArrayTopIndex; i++)
				{
					try
					{
						linesArray[i]->printNextStep();
					}
					catch(...)
					{
						//delete linesArray[i];
						canAllocateMemory = false;
						linesArrayCurrentIndex = 0;
					}
				}
			}
			refresh();
			if (nanosleep(&request, NULL) != 0)
			{
				throw GeneralException("nanosleep error.");
			}
		}
	}
	
	void ncursesEnd()
	{
		endwin();
	}
};

int main(int argc, char* argv[])
{
	int colorsArray[] = {COLOR_GREEN,0,COLOR_RED,0,COLOR_BLUE,0,COLOR_CYAN,0,COLOR_YELLOW,0,COLOR_MAGENTA,0,COLOR_WHITE,0};
	MainController controller;
	controller.readParameters();
	controller.ncursesStart(0,1,0,1,7,colorsArray);
	controller.drawingStart();
	//гипотетический controller.drawingStop...
	//controller.ncursesEnd();
	//exit(EXIT_SUCCESS);
}
