#ifndef _USERINPUTEXCEPTION_H_
#define _USERINPUTEXCEPTION_H_

#include "GeneralException.h"

class UserInputException : public GeneralException
{
public:
	UserInputException(string message) : GeneralException(message)
	{}
};

#endif // _USERINPUTEXCEPTION_H_