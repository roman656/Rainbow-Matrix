#ifndef _LISTEXCEPTION_H_
#define _LISTEXCEPTION_H_

#include "GeneralException.h"

class ListException : public GeneralException 
{
public:
	ListException(string message) : GeneralException(message)
	{}
};

#endif // _LISTEXCEPTION_H_

