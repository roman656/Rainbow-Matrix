#include "RandomSymbol.h"

RandomSymbol::RandomSymbol(bool isPolychromy) 
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

void RandomSymbol::printRandomSymbol(int yCoordinate, int xCoordinate)
{
	mvaddch(yCoordinate, xCoordinate, symbol_ | attribute_ | MY_ATTRIBUTES);
}