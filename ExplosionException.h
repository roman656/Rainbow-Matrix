#ifndef _EXPLOSIONEXCEPTION_H_
#define _EXPLOSIONEXCEPTION_H_

#include "GeneralException.h"

///Класс исключения взрывов.
class ExplosionException: public GeneralException 
{
public:
	///Конструктор.
	ExplosionException(std::string message) : GeneralException(message)
	{}
};

#endif // _EXPLOSIONEXCEPTION_H_