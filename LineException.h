#ifndef _LINEEXCEPTION_H_
#define _LINEEXCEPTION_H_

#include "GeneralException.h"

class LineException : public GeneralException 
{
public:
	LineException(string message) : GeneralException(message)
	{}
};

#endif // _LINEEXCEPTION_H_

