#include "MainController.h"

void MainController::readUserIntegerInput(short maxValue, short minValue, short& parameter)
{
	short input = 0;
	std::ostringstream exceptionOut;
    exceptionOut << "Wrong input! Try again [" << minValue << ";" << maxValue << "]: ";
	std::cin >> input;
	if (std::cin.fail())
	{
		std::cin.clear();
		while (std::cin.get() != '\n');
		throw UserInputException(exceptionOut.str());
	}
	else if ((std::cin.peek() != '\n') || (input > maxValue) || (input < minValue))
	{
		while (std::cin.get() != '\n');
		throw UserInputException(exceptionOut.str());
	}
	else
	{
		parameter = input;
	}
}

void MainController::readUserSymbolInput(bool& parameter)
{
	char input = 0;
	std::cin >> input;
	if (std::cin.fail())
	{
		std::cin.clear();
		while (std::cin.get() != '\n');
		throw UserInputException("Wrong input! Try again [Y/N]: ");
	}
	else if (std::cin.peek() != '\n')
	{
		while (std::cin.get() != '\n');
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
	std::string messagesArray[] = {"Enter the following parameters:\n","Size of line [1;30]: ","Line spawn frequency [1;30]: ","Symbol spawn frequency [1;30]: ","Do I use different colors [Y/N]: "};
	std::cout << messagesArray[0] << messagesArray[1];
	while (1)
	{
		try
		{		
			readUserIntegerInput(30, 1, lineSize);
			break;
		}
		catch (UserInputException& inputExc)
		{
			std::cout << inputExc.what();
		}
	}
	std::cout << messagesArray[2];
	while (1)
	{
		try
		{		
			readUserIntegerInput(30, 1, lineSpawnFrequency);
			break;
		}
		catch (UserInputException& inputExc)
		{
			std::cout << inputExc.what();
		}
	}
	std::cout << messagesArray[3];
	while (1)
	{
		try
		{		
			readUserIntegerInput(30, 1, symbolSpawnFrequency);
			break;
		}
		catch (UserInputException& inputExc)
		{
			std::cout << inputExc.what();
		}
	}
	std::cout << messagesArray[4];
	while (1)
	{
		try
		{		
			readUserSymbolInput(isPolychromy);
			break;
		}
		catch (UserInputException& inputExc)
		{
			std::cout << inputExc.what();
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
        short i = 0;
		short pairNo = 1;
		short check = nColorPairs;
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
	short xMaxCoordinate;
	short yMaxCoordinate;
	getmaxyx(stdscr, yMaxCoordinate, xMaxCoordinate);
	List <Line*> linesList;
	Line* temp = NULL;
	struct timespec sleepInterval, currentTime, tempInterval;
	sleepInterval.tv_sec = 0;
	sleepInterval.tv_nsec = 0;
	currentTime.tv_sec = 0;
	currentTime.tv_nsec = 0;
	tempInterval.tv_sec = 0;
	tempInterval.tv_nsec = 0;
	bool canGetUpdateTime = true;
	bool isTimeToSpawnLines = true;
	bool isTimeToSetBeginTime = true;
	long referenceTimeValue = 0;
	while (1)
	{
		if (clock_gettime(CLOCK_REALTIME, &currentTime) != 0)
		{
			int limit = linesList.getLength();
			for (int i = 0; i < limit; i++)
    		{
				delete linesList[i];
			}
			linesList.clear();
			endwin();
			fprintf(stderr,"In drawingStart() -> clock_gettime() error.\n");
			exit(EXIT_FAILURE);
		}
		if (isTimeToSetBeginTime)
		{
			isTimeToSetBeginTime = false;
			referenceTimeValue = currentTime.tv_sec * 1000000000 + currentTime.tv_nsec;
		}
		if ((currentTime.tv_sec * 1000000000 + currentTime.tv_nsec) - referenceTimeValue >= 999999998)
		{
			isTimeToSpawnLines = true;
			referenceTimeValue = currentTime.tv_sec * 1000000000 + currentTime.tv_nsec;
		}
		if (isTimeToSpawnLines)
		{
			for (int i = 0; i < lineSpawnFrequency; i++)
			{
				try
				{
					temp = new Line(isPolychromy, lineSize, rand() % xMaxCoordinate, yMaxCoordinate, symbolSpawnFrequency, currentTime);
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
				catch (std::bad_alloc& allocEx)
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
				(linesList[i])->printNextStep(currentTime);
				tempInterval = (linesList[i])->getUpdateTime();
				long difference = (tempInterval.tv_sec * 1000000000 + tempInterval.tv_nsec) - (currentTime.tv_sec * 1000000000 + currentTime.tv_nsec);
				if ((canGetUpdateTime) && (difference > 0))
				{
					canGetUpdateTime = false;
					sleepInterval.tv_nsec = difference % 1000000000;
					sleepInterval.tv_sec = difference / 1000000000;
					
				}
				else
				{
					if ((sleepInterval.tv_sec * 1000000000 + sleepInterval.tv_nsec) > difference)
					{
						sleepInterval.tv_nsec = difference % 1000000000;
						sleepInterval.tv_sec = difference / 1000000000;
					}
				}
			}
			catch (LineException& finishLineExc)
			{
				delete linesList[i];
				linesList.erase(i);
				i--;
			}
			catch (ListException& listEx)
			{
				int limit = linesList.getLength();
				for (int i = 0; i < limit; i++)
    			{
					delete linesList[i];
				}
				linesList.clear();
				endwin();
				fprintf(stderr,"Symbol spawner -> List error: %s\n", listEx.what());
				exit(EXIT_FAILURE);
			}
			catch (std::bad_alloc& allocEx)
			{
				int limit = linesList.getLength();
				for (int i = 0; i < limit; i++)
    			{
					delete linesList[i];
				}
				linesList.clear();
				endwin();
				fprintf(stderr,"Symbol spawner -> Memory error: %s\n", allocEx.what());
				exit(EXIT_FAILURE);
			}
		}
		canGetUpdateTime = true;
		refresh();
		if (nanosleep(&sleepInterval, NULL) != 0)
		{
			int limit = linesList.getLength();
			for (int i = 0; i < limit; i++)
    		{
				delete linesList[i];
			}
			linesList.clear();
			endwin();
			fprintf(stderr,"drawingStart() -> nanosleep() error.\nSeconds: %ld\nNanoseconds: %ld\n", sleepInterval.tv_sec, sleepInterval.tv_nsec);
			exit(EXIT_FAILURE);
		}
	}
}