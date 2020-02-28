#include "Line.h"

Line::Line(int lineSize, int xCoordinate, int yWindowBottomCoordinate, bool isPolychromy, int delay)
{
	size_ = lineSize;
	xCoordinate_ = xCoordinate;
	yTopCoordinate_ = 0;
	yBottomCoordinate_ = 0;
	yWindowBottomCoordinate_ = yWindowBottomCoordinate;
	isPolychromy_ = isPolychromy;
	delay_ = delay;
}

void Line::printNextStep() 
{
	if (delay_ > 0)
	{
		delay_--;
		return;
	}
	if ((yTopCoordinate_) >= yWindowBottomCoordinate_)
	{
		if ((yBottomCoordinate_ != yTopCoordinate_) && (yBottomCoordinate_ != 0))
		{
			mvaddch(yBottomCoordinate_, xCoordinate_, ' ');
			yBottomCoordinate_++;
		}
		else if ((yBottomCoordinate_ != yTopCoordinate_) && (yBottomCoordinate_ == 0))
		{
			if ((yTopCoordinate_ - yBottomCoordinate_) < size_)
			{
				size_--;
			}
			else
			{
				mvaddch(yBottomCoordinate_, xCoordinate_, ' ');
				yBottomCoordinate_++;
			}
		}
		else
		{
			throw LineException("The line has completed its task.");
		}
	}
	else
	{
		if ((yTopCoordinate_ - yBottomCoordinate_) >= size_)
		{
			mvaddch(yBottomCoordinate_, xCoordinate_, ' ');
			yBottomCoordinate_++;
		}
		RandomSymbol* temp = new RandomSymbol(isPolychromy_);
		temp->printRandomSymbol(yTopCoordinate_, xCoordinate_);
		delete temp;
		yTopCoordinate_++;
	}
}