#ifndef _MAINCONTROLLER_H_
#define _MAINCONTROLLER_H_

#include <list>
#include "UserInputException.h"
#include "Line.h"

#define DEFAULT_POLYCHROMY false
#define DEFAULT_SYMBOL_SPAWN_FREQUENCY 30
#define DEFAULT_LINE_SPAWN_FREQUENCY 30
#define DEFAULT_LINE_SIZE 8

class MainController
{
private:
	int lineSize_;
	int lineSpawnFrequency_;
	int symbolSpawnFrequency_;
	bool isPolychromy_;

	void readNUserInputCharacters_(char* inputArray, int inputArraySize, int numberOfCharactersToRead);
	void determUserIntegerInput_(char* inputArray, int maxValue, int minValue, int& parameter);
	void determUserSymbolInput_(char* inputArray, bool& parameter);
public:
	MainController();
	void readParameters();
	void ncursesStart(bool useCursor, bool useKeypad, bool useEcho, bool useColor, int nColorPairs, int* colorsArray);
	void drawingStart();
};


#endif // _MAINCONTROLLER_H_