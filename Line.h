#ifndef _LINE_H_
#define _LINE_H_

#include "LineException.h"
#include "RandomSymbol.h"

class Line
{
private:
	int delay_;
	int size_;
	int xCoordinate_;
	int yTopCoordinate_;
	int yBottomCoordinate_;
	int yWindowBottomCoordinate_;
	bool isPolychromy_;
public:
	Line(int lineSize, int xCoordinate, int yWindowBottomCoordinate, bool isPolychromy, int delay);
	void printNextStep();
};

#endif // _LINE_H_