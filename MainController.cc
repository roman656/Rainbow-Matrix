#include "MainController.h"

void MainController::readNUserInputCharacters_(char* inputArray, int inputArraySize, int numberOfCharactersToRead)
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

void MainController::determUserIntegerInput_(char* inputArray, int maxValue, int minValue, int& parameter)
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
		throw UserInputException("Wrong input! Try again [1;30]: ");
	}
	int value = atoi(inputArray);
	if ((value > maxValue) || (value < minValue))
	{
		throw UserInputException("Wrong value! Try again [1;30]: ");
	}
	parameter = value;
}

void MainController::determUserSymbolInput_(char* inputArray, bool& parameter)
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
		throw UserInputException("Wrong input! Try again [Y/N]: ");
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
		throw UserInputException("Wrong value! Try again [Y/N]: ");
	}
}

MainController::MainController() 
{
	lineSize_ = DEFAULT_LINE_SIZE;
	isPolychromy_ = DEFAULT_POLYCHROMY;
	lineSpawnFrequency_ = DEFAULT_LINE_SPAWN_FREQUENCY;
	symbolSpawnFrequency_ = DEFAULT_SYMBOL_SPAWN_FREQUENCY;
	srand(time(NULL));
}

void MainController::readParameters()
{
	string messagesArray[] = {"Enter the following parameters:\n","Size of line [1;30]: ","Line spawn frequency [1;30]: ","Symbol spawn frequency [1;30]: ","Do I use different colors [Y/N]: "};
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
					temp = new Line(lineSize_, rand() % xMaxCoordinate, yMaxCoordinate, isPolychromy_, rand() % symbolSpawnFrequency_);
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