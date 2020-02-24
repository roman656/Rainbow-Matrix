#include <stdexcept>
#include <list>
//#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>
//#include <ctype.h>
//#include <string.h>
//#include <unistd.h>
#include <ncurses.h>

#define DEFAULT_POLYCHROMY true
#define DEFAULT_SYMBOL_SPAWN_FREQUENCY 3
#define DEFAULT_LINE_SPAWN_FREQUENCY 5
#define DEFAULT_LINE_SIZE 5
#define DEFAULT_COLOR_PAIR 1
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

class LineException : public GeneralException
{
public:
	LineException(string message) : GeneralException(message)
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
		if ((yTopCoordinate_) >= yWindowBottomCoordinate_)
		{
			if ((yBottomCoordinate_ != yTopCoordinate_) && (yBottomCoordinate_ != 0))
			{
				mvaddch(yBottomCoordinate_, xCoordinate_, CLEANER);
				yBottomCoordinate_++;
			}
			else if ((yBottomCoordinate_ != yTopCoordinate_) && (yBottomCoordinate_ == 0))
			{
				if ((yTopCoordinate_ - yBottomCoordinate_) < size_)
				{
					size_--;
				}
				else
				{
					mvaddch(yBottomCoordinate_, xCoordinate_, CLEANER);
					yBottomCoordinate_++;
				}
			}
			else
			{
				throw LineException("The line has completed its task.");
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

	void readNUserInputCharacters_(char* inputArray, int inputArraySize, int numberOfCharactersToRead)
	{
		if (inputArraySize <= numberOfCharactersToRead)
		{
			throw GeneralException("readNUserInputCharacters_() -> Input array size is too small.\n");
		}
		inputArray[0] = 0;
		int c;
		int i = 0;
		while (numberOfCharactersToRead > 0)
		{
			c = fgetc(stdin);
			if (c == '\n')
			{
				inputArray[i++] = c;
				inputArray[i] = 0;
				return;
			}
			inputArray[i++] = c;
			inputArray[i] = 0;
			numberOfCharactersToRead--;
		}
		while (fgetc(stdin) != '\n') {}
	}

	void determUserIntegerInput_(char* inputArray, int maxValue, int minValue, int& parameter)
	{
		int i = 0;
		bool isCorrect = false;
		while (1)
		{
			if (inputArray[i] == '\0')
			{
				break;
			}
			else if ((inputArray[i] == '\n') && (i != 0))
			{
				isCorrect = true;
				break;
			}
			else if (isdigit(inputArray[i]) == 0)
			{
				break;
			}
			i++;
		}
		if (!isCorrect)
		{
			throw UserInputException("Wrong input! Try again: ");
		}
		int value = atoi(inputArray);
		if ((value > maxValue) || (value < minValue))
		{
			throw UserInputException("Wrong value! Try again: ");
		}
		parameter = value;
	}

	void determUserSymbolInput_(char* inputArray, bool& parameter)
	{
		int i = 0;
		bool isCorrect = false;
		while (1)
		{
			if (inputArray[i] == '\0')
			{
				break;
			}
			else if ((inputArray[i] == '\n') && (i == 1))
			{
				isCorrect = true;
				break;
			}
			else if (isalpha(inputArray[i]) == 0)
			{
				break;
			}
			i++;
		}
		if (!isCorrect)
		{
			throw UserInputException("Wrong input! Try again: ");
		}
		if ((inputArray[0] == 'Y') || (inputArray[0] == 'y'))
		{
			parameter = true;
		}
		else if ((inputArray[0] == 'N') || (inputArray[0] == 'n'))
		{
			parameter = false;
		}
		else
		{
			throw UserInputException("Wrong value! Try again: ");
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
		string messagesArray[] = {"Enter the following parameters:\n","Size of line: ","Line spawn frequency: ","Symbol spawn frequency: ","Do I use different colors [Y/N]: "};
		char userInputArray[4];
		printf("%s",messagesArray[0].c_str());
		printf("%s",messagesArray[1].c_str());
		while (1)
		{
			try
			{		
				readNUserInputCharacters_(userInputArray, 4, 3);
				determUserIntegerInput_(userInputArray, 30, 1, lineSize_);
				break;
			}
			catch (UserInputException& inputExc)
			{
				printf("%s", inputExc.what());
			}
			catch (GeneralException& parametersExc)
			{
				fprintf(stderr,"%s", parametersExc.what());
				exit(EXIT_FAILURE);
			}
		}
		printf("%s",messagesArray[2].c_str());
		while (1)
		{
			try
			{		
				readNUserInputCharacters_(userInputArray, 4, 3);
				determUserIntegerInput_(userInputArray, 30, 1, lineSpawnFrequency_);
				break;
			}
			catch (UserInputException& inputExc)
			{
				printf("%s", inputExc.what());
			}
			catch (GeneralException& parametersExc)
			{
				fprintf(stderr,"%s", parametersExc.what());
				exit(EXIT_FAILURE);
			}
		}
		printf("%s",messagesArray[3].c_str());
		while (1)
		{
			try
			{		
				readNUserInputCharacters_(userInputArray, 4, 3);
				determUserIntegerInput_(userInputArray, 30, 1, symbolSpawnFrequency_);
				break;
			}
			catch (UserInputException& inputExc)
			{
				printf("%s", inputExc.what());
			}
			catch (GeneralException& parametersExc)
			{
				fprintf(stderr,"%s", parametersExc.what());
				exit(EXIT_FAILURE);
			}
		}
		printf("%s",messagesArray[4].c_str());
		while (1)
		{
			try
			{		
				readNUserInputCharacters_(userInputArray, 4, 2);
				determUserSymbolInput_(userInputArray, isPolychromy_);
				break;
			}
			catch (UserInputException& inputExc)
			{
				printf("%s", inputExc.what());
			}
			catch (GeneralException& parametersExc)
			{
				fprintf(stderr,"%s", parametersExc.what());
				exit(EXIT_FAILURE);
			}
		}
	}
	
	void ncursesStart(bool useCursor, bool useKeypad, bool useEcho, bool useColor, int nColorPairs, int* colorsArray)
	{
		if (!initscr()) 
		{
			fprintf(stderr,"ncursesStart() -> Initialization error.\n");
			exit(EXIT_FAILURE);
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
				endwin();
				fprintf(stderr,"ncursesStart() -> This terminal does not support colors.\n");
				exit(EXIT_FAILURE);
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
		list <Line*> linesList;
		Line* temp = NULL;
		struct timespec request;
		request.tv_sec = 0;
		request.tv_nsec = 999999998 / symbolSpawnFrequency_;
		bool isTimeToSpawnLines = true;
		int timeIntervalCounter = symbolSpawnFrequency_;
		while (1)
		{
			if (timeIntervalCounter == symbolSpawnFrequency_)
			{
				isTimeToSpawnLines = true;
				timeIntervalCounter = 0;
			}
			if (isTimeToSpawnLines)
			{
				for (int i = 0; i < lineSpawnFrequency_; i++)
				{
					try
					{
						temp = new Line(lineSize_, rand() % xMaxCoordinate, yMaxCoordinate, isPolychromy_);
						linesList.push_back(temp);
						temp = NULL;
					}
					catch (bad_alloc& allocEx)
					{
						for (auto iterator = linesList.begin(); iterator != linesList.end(); iterator++)
    					{
							delete *iterator;
						}
						linesList.clear();
						endwin();
						fprintf(stderr,"Lines spawner -> Memory error: %s\n",allocEx.what());
						exit(EXIT_FAILURE);
					}
				}
				isTimeToSpawnLines = false;
			}
			for (auto iterator = linesList.begin(); iterator != linesList.end(); iterator++)
    		{
				try
				{
					(*iterator)->printNextStep();
				}
				catch(LineException& finishLineExc)
				{
					delete *iterator;
					iterator = linesList.erase(iterator);
					iterator--;
				}
				catch (bad_alloc& allocEx)
				{
					for (auto iterator = linesList.begin(); iterator != linesList.end(); iterator++)
    				{
						delete *iterator;
					}
					linesList.clear();
					endwin();
					fprintf(stderr,"Symbol spawner -> Memory error: %s\n",allocEx.what());
					exit(EXIT_FAILURE);
				}
			}
			refresh();
			if (nanosleep(&request, NULL) != 0)
			{
				for (auto iterator = linesList.begin(); iterator != linesList.end(); iterator++)
    			{
					delete *iterator;
				}
				linesList.clear();
				endwin();
				fprintf(stderr,"drawingStart() -> nanosleep() error.\n");
				exit(EXIT_FAILURE);
			}
			timeIntervalCounter++;
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
