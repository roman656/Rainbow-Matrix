#ifndef _LINE_H_
#define _LINE_H_

#include "LineException.h"
#include "RandomSymbol.h"

class Line
{
private:
	int delay;
	int size;
	int xCoordinate;
	int yTopCoordinate;
	int yBottomCoordinate;
	int yWindowBottomCoordinate;
	bool isPolychromy;
public:
	Line(int lineSize, int xCoordinate, int yWindowBottomCoordinate, bool isPolychromy, int delay);
	void printNextStep();
};

#endif // _LINE_H_