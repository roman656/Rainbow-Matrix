#ifndef _MAINCONTROLLER_H_
#define _MAINCONTROLLER_H_

#include "UserInputException.h"
#include "Line.h"
#include "List.h"
#include <iostream>

#define DEFAULT_POLYCHROMY true
#define DEFAULT_SYMBOL_SPAWN_FREQUENCY 10
#define DEFAULT_LINE_SPAWN_FREQUENCY 30
#define DEFAULT_LINE_SIZE 1

class MainController
{
private:
	int lineSize;
	int lineSpawnFrequency;
	int symbolSpawnFrequency;
	bool isPolychromy;

	void readUserIntegerInput(int maxValue, int minValue, int& parameter);
	void readUserSymbolInput(bool& parameter);
public:
	MainController();
	void readParameters();
	void ncursesStart(bool useCursor, bool useKeypad, bool useEcho, bool useColor, int nColorPairs, int* colorsArray);
	void drawingStart();
};


#endif // _MAINCONTROLLER_H_