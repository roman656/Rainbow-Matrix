#ifndef _GENERALEXCEPTION_H_
#define _GENERALEXCEPTION_H_

#include <stdexcept>

using namespace std;

class GeneralException : public exception 
{
private:
	string errorMessage;
public:
	GeneralException(string message);
	const char* what() const noexcept override;
};

#endif // _GENERALEXCEPTION_H_