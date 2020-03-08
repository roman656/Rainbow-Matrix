#include "MainController.h"

void MainController::readUserIntegerInput(int maxValue, int minValue, int& parameter)
{
	int input = 0;
	cin >> input;
	if (cin.fail())
	{
		cin.clear();
		while (cin.get() != '\n');
		throw UserInputException("Wrong input! Try again [1;30]: ");
	}
	else if ((cin.peek() != '\n') || (input > maxValue) || (input < minValue))
	{
		while (cin.get() != '\n');
		throw UserInputException("Wrong input! Try again [1;30]: ");
	}
	else
	{
		parameter = input;
	}
}

void MainController::readUserSymbolInput(bool& parameter)
{
	char input = 0;
	cin >> input;
	if (cin.fail())
	{
		cin.clear();
		while (cin.get() != '\n');
		throw UserInputException("Wrong input! Try again [Y/N]: ");
	}
	else if (cin.peek() != '\n')
	{
		while (cin.get() != '\n');
		throw UserInputException("Wrong input! Try again [Y/N]: ");
	}
	else
	{
		if ((input == 'Y') || (input == 'y'))
		{
			parameter = true;
		}
		else if ((input == 'N') || (input == 'n'))
		{
			parameter = false;
		}
		else
		{
			throw UserInputException("Wrong input! Try again [Y/N]: ");
		}
	}
}

MainController::MainController() 
{
	lineSize = DEFAULT_LINE_SIZE;
	isPolychromy = DEFAULT_POLYCHROMY;
	lineSpawnFrequency = DEFAULT_LINE_SPAWN_FREQUENCY;
	symbolSpawnFrequency = DEFAULT_SYMBOL_SPAWN_FREQUENCY;
	srand(time(NULL));
}

void MainController::readParameters()
{
	string messagesArray[] = {"Enter the following parameters:\n","Size of line [1;30]: ","Line spawn frequency [1;30]: ","Symbol spawn frequency [1;30]: ","Do I use different colors [Y/N]: "};
	printf("%s",messagesArray[0].c_str());
	printf("%s",messagesArray[1].c_str());
	while (1)
	{
		try
		{		
			readUserIntegerInput(30, 1, lineSize);
			break;
		}
		catch (UserInputException& inputExc)
		{
			printf("%s", inputExc.what());
		}
	}
	printf("%s",messagesArray[2].c_str());
	while (1)
	{
		try
		{		
			readUserIntegerInput(30, 1, lineSpawnFrequency);
			break;
		}
		catch (UserInputException& inputExc)
		{
			printf("%s", inputExc.what());
		}
	}
	printf("%s",messagesArray[3].c_str());
	while (1)
	{
		try
		{		
			readUserIntegerInput(30, 1, symbolSpawnFrequency);
			break;
		}
		catch (UserInputException& inputExc)
		{
			printf("%s", inputExc.what());
		}
	}
	printf("%s",messagesArray[4].c_str());
	while (1)
	{
		try
		{		
			readUserSymbolInput(isPolychromy);
			break;
		}
		catch (UserInputException& inputExc)
		{
			printf("%s", inputExc.what());
		}
	}
}
	
void MainController::ncursesStart(bool useCursor, bool useKeypad, bool useEcho, bool useColor, int nColorPairs, int* colorsArray)
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
	
void MainController::drawingStart()
{
	int xMaxCoordinate;
	int yMaxCoordinate;
	getmaxyx(stdscr, yMaxCoordinate, xMaxCoordinate);
	List <Line*> linesList;
	Line* temp = NULL;
	struct timespec request;
	request.tv_sec = 0;
	request.tv_nsec = 999999998 / symbolSpawnFrequency;
	bool isTimeToSpawnLines = true;
	int timeIntervalCounter = symbolSpawnFrequency;
	while (1)
	{
		if (timeIntervalCounter == symbolSpawnFrequency)
		{
			isTimeToSpawnLines = true;
			timeIntervalCounter = 0;
		}
		if (isTimeToSpawnLines)
		{
			for (int i = 0; i < lineSpawnFrequency; i++)
			{
				try
				{
					temp = new Line(lineSize, rand() % xMaxCoordinate, yMaxCoordinate, isPolychromy, rand() % symbolSpawnFrequency);
					linesList.push_back(temp);
					temp = NULL;
				}
				catch (ListException& ListEx)
				{
					int limit = linesList.getLength();
					for (int i = 0; i < limit; i++)
    				{
						delete linesList[i];
					}
					linesList.clear();
					endwin();
					fprintf(stderr,"Lines spawner -> %s",ListEx.what());
					exit(EXIT_FAILURE);
				}
				catch (bad_alloc& allocEx)
				{
					int limit = linesList.getLength();
					for (int i = 0; i < limit; i++)
    				{
						delete linesList[i];
					}
					linesList.clear();
					endwin();
					fprintf(stderr,"Lines spawner -> Memory error: %s\n",allocEx.what());
					exit(EXIT_FAILURE);
				}
			}
			isTimeToSpawnLines = false;
		}
		for (int i = 0; i < linesList.getLength(); i++)
    	{
			try
			{
				(linesList[i])->printNextStep();
			}
			catch(LineException& finishLineExc)
			{
				delete linesList[i];
				linesList.erase(i);
				i--;
			}
			catch (bad_alloc& allocEx)
			{
				int limit = linesList.getLength();
				for (int i = 0; i < limit; i++)
    			{
					delete linesList[i];
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
			int limit = linesList.getLength();
			for (int i = 0; i < limit; i++)
    		{
				delete linesList[i];
			}
			linesList.clear();
			endwin();
			fprintf(stderr,"drawingStart() -> nanosleep() error. Value: %ld\n", request.tv_nsec);
			exit(EXIT_FAILURE);
		}
		timeIntervalCounter++;
	}
}