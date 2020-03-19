#include "GeneralException.h"

GeneralException::GeneralException(std::string message)
{
	errorMessage = message;
}
	
const char* GeneralException::what() const noexcept
{
	return errorMessage.c_str();
}