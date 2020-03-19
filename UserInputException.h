#ifndef _USERINPUTEXCEPTION_H_
#define _USERINPUTEXCEPTION_H_

#include "GeneralException.h"

///Класс ислючения для пользовательсого ввода.
class UserInputException : public GeneralException
{
public:
	///Конструктор.
	UserInputException(std::string message) : GeneralException(message)
	{}
};

#endif // _USERINPUTEXCEPTION_H_