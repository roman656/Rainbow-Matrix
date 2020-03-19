#ifndef _LINEEXCEPTION_H_
#define _LINEEXCEPTION_H_

#include "GeneralException.h"

///Класс исключения линий.
class LineException : public GeneralException 
{
public:
	///Конструктор.
	LineException(std::string message) : GeneralException(message)
	{}
};

#endif // _LINEEXCEPTION_H_

