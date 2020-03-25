#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "RandomSymbol.h"
#include "ExplosionException.h"

///Класс взрыва.
class Explosion
{
private:
	bool isPolychromy;			//Флаг: состоит ли взрыв из разноцветных символов.
	short xCenterCoordinate;		//Координата центра взрыва по оси OX.
	short yCenterCoordinate;		//Координата центра взрыва по оси OY.
	short finalExplosionRadius;		//Конечный радиус взрыва.
	short currentExplosionRadius;		//Текущий радиус взрыва.
	struct timespec updateTime;		//Время (секнды и наносекунды) после которого необходимо обновить состояние взрыва. 
	struct timespec updateTimeStep;		//Временной шаг, позволяющй взрыву расходиться с постоянной скоростью.
public:
	///Конструктор.
	///Входные параметры:
	///\param bool isPolychromy - флаг: состоит ли взрыв из разноцветных символов;
	///\param short xCenterCoordinate - координата центра взрыва по оси OX;
	///\param short yCenterCoordinate - координата центра взрыва по оси OY;
	///\param short explosionRadius - конечный радиус взрыва;
	///\param short explosionRadialSpeed - радиальная скорость взрыва (частота появления символов);
	///\param struct timespec currentTime - текущее время (секнды и наносекунды).
	Explosion(bool isPolychromy, short xCenterCoordinate, short yCenterCoordinate, short explosionRadius, short explosionRadialSpeed, struct timespec currentTime);

	///Метод, обновляющий состояние взрыва (метод сам определяет, в зависимости от полученного параметра, какие действия производить).
	///Входные параметры:
	///\param struct timespec currentTime - текущее время (секнды и наносекунды).
	void printNextStep(struct timespec currentTime);

	///Геттер, возвращающий updateTime.
	struct timespec getUpdateTime();
};

#endif // _EXPLOSION_H_
