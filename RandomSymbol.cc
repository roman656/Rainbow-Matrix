#include "RandomSymbol.h"

RandomSymbol::RandomSymbol(bool isPolychromy) 
{
	symbol = rand() % 94 + 33;
	if (isPolychromy)
	{
		attribute = COLOR_PAIR(rand() % 7 + 1);
	}
	else
	{
		attribute = COLOR_PAIR(DEFAULT_COLOR_PAIR);
	}
}

void RandomSymbol::printRandomSymbol(int yCoordinate, int xCoordinate)
{
	mvaddch(yCoordinate, xCoordinate, symbol | attribute | MY_ATTRIBUTES);
}