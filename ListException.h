#ifndef _LISTEXCEPTION_H_
#define _LISTEXCEPTION_H_

#include "GeneralException.h"

///Класс исключения для списка.
class ListException : public GeneralException 
{
public:
	///Конструктор.
	ListException(std::string message) : GeneralException(message)
	{}
};

#endif // _LISTEXCEPTION_H_

