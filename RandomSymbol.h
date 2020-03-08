#ifndef _RANDOMSYMBOL_H_
#define _RANDOMSYMBOL_H_

#include <ncurses.h>
#include <cstdlib>

#define MY_ATTRIBUTES A_BOLD   //Default value: A_NORMAL
#define DEFAULT_COLOR_PAIR 1

using namespace std;

class RandomSymbol
{
private:
	char symbol;
	int attribute;
public:
	RandomSymbol(bool isPolychromy);
	void printRandomSymbol(int yCoordinate, int xCoordinate);
};

#endif // _RANDOMSYMBOL_H_