#include "Line.h"

Line::Line(int lineSize, int xCoordinate, int yWindowBottomCoordinate, bool isPolychromy, int delay)
{
	this->size = lineSize;
	this->xCoordinate = xCoordinate;
	this->yTopCoordinate = 0;
	this->yBottomCoordinate= 0;
	this->yWindowBottomCoordinate = yWindowBottomCoordinate;
	this->isPolychromy = isPolychromy;
	this->delay = delay;
}

void Line::printNextStep() 
{
	if (delay > 0)
	{
		delay--;
		return;
	}
	if ((yTopCoordinate) >= yWindowBottomCoordinate)
	{
		if ((yBottomCoordinate != yTopCoordinate) && (yBottomCoordinate != 0))
		{
			mvaddch(yBottomCoordinate, xCoordinate, ' ');
			yBottomCoordinate++;
		}
		else if ((yBottomCoordinate != yTopCoordinate) && (yBottomCoordinate == 0))
		{
			if ((yTopCoordinate - yBottomCoordinate) < size)
			{
				size--;
			}
			else
			{
				mvaddch(yBottomCoordinate, xCoordinate, ' ');
				yBottomCoordinate++;
			}
		}
		else
		{
			throw LineException("The line has completed its task.");
		}
	}
	else
	{
		if ((yTopCoordinate - yBottomCoordinate) >= size)
		{
			mvaddch(yBottomCoordinate, xCoordinate, ' ');
			yBottomCoordinate++;
		}
		RandomSymbol* temp = new RandomSymbol(isPolychromy);
		temp->printRandomSymbol(yTopCoordinate, xCoordinate);
		delete temp;
		yTopCoordinate++;
	}
}