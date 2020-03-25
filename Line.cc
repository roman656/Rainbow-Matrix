#include "Line.h"

Line::Line(bool isPolychromy, short lineSize, short xCoordinate, short yWindowBottomCoordinate, short symbolSpawnFrequency, short lineExplosionProbability, struct timespec currentTime)
{
	this->isPolychromy = isPolychromy;
	this->canExplode = false;
	this->size = lineSize;
	this->xCoordinate = xCoordinate;
	this->yTopCoordinate = 0;
	this->yBottomCoordinate = 0;
	this->lineExplosionProbability = lineExplosionProbability;
	this->yWindowBottomCoordinate = yWindowBottomCoordinate;
	this->updateTimeStep.tv_sec = 0;
	this->updateTimeStep.tv_nsec = 999999998 / symbolSpawnFrequency;
	struct timespec delay;
	delay.tv_sec = 0;
	delay.tv_nsec = rand() % 999999998;
	if (delay.tv_nsec + currentTime.tv_nsec >= 999999998)
	{
		delay.tv_sec = 1;
		delay.tv_nsec = delay.tv_nsec - 999999998;
	}
	this->updateTime.tv_sec = currentTime.tv_sec + delay.tv_sec;
	this->updateTime.tv_nsec = currentTime.tv_nsec + delay.tv_nsec;
}

void Line::printNextStep(struct timespec currentTime, bool& wasExplosion) 
{
	if ((currentTime.tv_sec * 1000000000 + currentTime.tv_nsec) >= (updateTime.tv_sec * 1000000000 + updateTime.tv_nsec)) 
	{
		if (0 >= size)
		{
			throw LineException("The line has completed its task.");
		}
		if ((yTopCoordinate) >= yWindowBottomCoordinate)
		{
			canExplode = false;
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
			canExplode = true;
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
		if (updateTimeStep.tv_nsec + currentTime.tv_nsec > 999999998)	//Перезапись времени обновления. Данное условие отвечает за корректную запись времени.
		{
			updateTime.tv_sec = currentTime.tv_sec + 1;
			updateTime.tv_nsec = abs(updateTimeStep.tv_nsec + currentTime.tv_nsec - 1000000000);
		}
		else
		{
			updateTime.tv_sec = currentTime.tv_sec;
			updateTime.tv_nsec = updateTimeStep.tv_nsec + currentTime.tv_nsec;
		}
		if (((rand() % 100 + 1) > (100 - lineExplosionProbability)) && canExplode)
		{
			wasExplosion = true;
			yTopCoordinate--;
			size--;
			mvaddch(yTopCoordinate, xCoordinate, ' ');
		}
		else
		{
			wasExplosion = false;
		}
	}
}

struct timespec Line::getUpdateTime()
{
	return updateTime;
}

short Line::getXCoordinate()
{
	return xCoordinate;
}

short Line::getYTopCoordinate()
{
	return yTopCoordinate;
}