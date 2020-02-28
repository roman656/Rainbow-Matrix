#include "GeneralException.h"

GeneralException::GeneralException(string message)
{
	errorMessage_ = message;
}
	
const char* GeneralException::what() const noexcept
{
	return errorMessage_.c_str();
}