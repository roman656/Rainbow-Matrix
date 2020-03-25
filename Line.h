#ifndef _LINE_H_
#define _LINE_H_

#include "LineException.h"
#include "RandomSymbol.h"

///Класс линии.
class Line
{
private:
	bool canExplode;			//Флаг: может ли на данном этапе произойти взрыв.
	bool isPolychromy;			//Флаг: состоит ли линия из разноцветных символов.
	short size;				//Размер линии (число символов).
	short xCoordinate;			//Координата линии по оси OX.
	short yTopCoordinate;			//1 + координата первого символа линии по оси OY (Пример: если на экране отображен 1й символ (координата по OY = 0), то значение данной переменной равно 1).
	short yBottomCoordinate;		//Координата последнего символа линии по оси OY.
	short yWindowBottomCoordinate;		//Максимально допустимая координата по оси OY.
	short lineExplosionProbability;		//Вероятность взрыва линии (%) [0;100].
	struct timespec updateTime;		//Время (секнды и наносекунды) после которого необходимо обновить состояние линии (Например, напечатать следующий символ). 
	struct timespec updateTimeStep;		//Временной шаг, позволяющй линии двигаться с постоянной скоростью.
public:
	///Конструктор.
	///Входные параметры:
	///\param bool isPolychromy - флаг: состоит ли линия из разноцветных символов;
	///\param short lineSize - размер линии (число символов);
	///\param short xCoordinate - координата линии по оси OX;
	///\param short yWindowBottomCoordinate - максимально допустимая координата по оси OY;
	///\param short symbolSpawnFrequency - скорость линии (частота появления символов). Число символов, которые должны быть напечатаны за еденицу времени;
	///\param short lineExplosionProbability - вероятность взрыва линии (%) [0;100];
	///\param struct timespec currentTime - текущее время (секнды и наносекунды).
	Line(bool isPolychromy, short lineSize, short xCoordinate, short yWindowBottomCoordinate, short symbolSpawnFrequency, short lineExplosionProbability, struct timespec currentTime);	

	///Метод, обновляющий состояние линии (метод сам определяет, в зависимости от полученных параметров, какие действия производить).
	///Входные параметры:
	///\param struct timespec currentTime - текущее время (секнды и наносекунды);
	///\param bool& wasExplosion - ссылка на флаг, указывающий на появление взрыва.
	void printNextStep(struct timespec currentTime, bool& wasExplosion);	

	///Геттер, возвращающий updateTime.
	struct timespec getUpdateTime();	

	///Геттер, возвращающий xCoordinate.
	short getXCoordinate();

	///Геттер, возвращающий yTopCoordinate.
	short getYTopCoordinate();
};

#endif // _LINE_H_
