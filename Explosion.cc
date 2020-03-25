#include "Explosion.h"

Explosion::Explosion(bool isPolychromy, short xCenterCoordinate, short yCenterCoordinate, short explosionRadius, short explosionRadialSpeed, struct timespec currentTime)
{
	this->isPolychromy = isPolychromy;
	this->xCenterCoordinate = xCenterCoordinate;	
	this->yCenterCoordinate = yCenterCoordinate;	
	this->finalExplosionRadius = explosionRadius;
	this->currentExplosionRadius = 0;
	this->updateTimeStep.tv_sec = 0;
	this->updateTimeStep.tv_nsec = 999999998 / explosionRadialSpeed;
	this->updateTime.tv_sec = currentTime.tv_sec;	
	this->updateTime.tv_nsec = currentTime.tv_nsec;
}

void Explosion::printNextStep(struct timespec currentTime)
{
	if ((currentTime.tv_sec * 1000000000 + currentTime.tv_nsec) >= (updateTime.tv_sec * 1000000000 + updateTime.tv_nsec))	//Условие отрисовки следующего шага взрыва.
	{
		short directionSwitch;		//Отвечает за направление, в котором будет производиться печать (регулирует изменения координат).
		short directionCounter;		//Счетчик, переключающий directionSwitch через определенное число итераций цикла печати.
		short yCoordinate;		//Координата печатаемого символа по оси OY.
		short xCoordinate;		//Координата печатаемого символа по оси OX.
		short yMaxCoordinate;		//Максимальная координата по оси OY (печать по данной координате недопустима).
		short xMaxCoordinate;		//Максимальная координата по оси OX (печать по данной координате недопустима).
		getmaxyx(stdscr, yMaxCoordinate, xMaxCoordinate);	//Получение значений yMaxCoordinate и xMaxCoordinate.
		if (currentExplosionRadius > 0)				//Условие необходимости затирания прошлого контура взрыва.
		{
			short nRandomSymbolsAtPreviousStep = currentExplosionRadius * 4;	//Необходимое количество случайных символов для отображения взрыва на прошлом шаге.
			directionSwitch = 0;
			directionCounter = 0;
			yCoordinate = yCenterCoordinate;
			xCoordinate = xCenterCoordinate - currentExplosionRadius;
			for (short i = 0; i < nRandomSymbolsAtPreviousStep; i++)
			{
				if ((yCoordinate >= 0) && (xCoordinate >= 0) && (yCoordinate < yMaxCoordinate) && (xCoordinate < xMaxCoordinate))
				{
					mvaddch(yCoordinate, xCoordinate, ' ');
				}
				switch (directionSwitch)
				{
					case 0:		//Направление положительное по оси OX и отрицательное по оси OY (Ось OY направлена вниз).
					{
						yCoordinate--;
						xCoordinate++;
						break;
					}
					case 1:		//Направление положительное по осям OX и OY.
					{
						yCoordinate++;
						xCoordinate++;
						break;
					}
					case 2:		//Направление положительное по оси OY и отрицательное по оси OX.
					{
						yCoordinate++;
						xCoordinate--;
						break;
					}
					case 3:		//Направление отрицательное по осям OX и OY.
					{
						yCoordinate--;
						xCoordinate--;
					}
				}
				if (directionCounter == currentExplosionRadius - 1)
				{
					directionCounter = -1;
					directionSwitch++;
				}
				directionCounter++;
			}
		}
		currentExplosionRadius++;				//Печать нового контура взрыва.
		if (currentExplosionRadius > finalExplosionRadius)	//Если радиус нового взрыва больше максимально допустимого - весь взрыв был отрисован и пора освободить от него память.
		{
			throw ExplosionException("The explosion has completed its task.");
		}
		short nRandomSymbolsAtThisStep = currentExplosionRadius * 4;	//Необходимое количество случайных символов для отображения взрыва на данном шаге.
		RandomSymbol* temp = nullptr;
		directionSwitch = 0;
		directionCounter = 0;
		yCoordinate = yCenterCoordinate;
		xCoordinate = xCenterCoordinate - currentExplosionRadius;
		for (short i = 0; i < nRandomSymbolsAtThisStep; i++)
		{
			if ((yCoordinate >= 0) && (xCoordinate >= 0) && (yCoordinate < yMaxCoordinate) && (xCoordinate < xMaxCoordinate))
			{
				temp = new RandomSymbol(isPolychromy);
				temp->printRandomSymbol(yCoordinate, xCoordinate);
				delete temp;
				temp = nullptr;
			}
			switch (directionSwitch)
			{
				case 0:		//Направление положительное по оси OX и отрицательное по оси OY (Ось OY направлена вниз).
				{
					yCoordinate--;
					xCoordinate++;
					break;
				}
				case 1:		//Направление положительное по осям OX и OY.
				{
					yCoordinate++;
					xCoordinate++;
					break;
				}
				case 2:		//Направление положительное по оси OY и отрицательное по оси OX.
				{
					yCoordinate++;
					xCoordinate--;
					break;
				}
				case 3:		//Направление отрицательное по осям OX и OY.
				{
					yCoordinate--;
					xCoordinate--;
				}
			}
			if (directionCounter == currentExplosionRadius - 1)
			{
				directionCounter = -1;
				directionSwitch++;
			}
			directionCounter++;
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
	}
}

struct timespec Explosion::getUpdateTime()
{
	return updateTime;
}
