#include "GeneralException.h"

GeneralException::GeneralException(string message)
{
	errorMessage = message;
}
	
const char* GeneralException::what() const noexcept
{
	return errorMessage.c_str();
}