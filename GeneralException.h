#ifndef _GENERALEXCEPTION_H_
#define _GENERALEXCEPTION_H_

/*---Библиотеки-Начало---*/
#include <stdexcept>
/*---Библиотеки-Конец---*/
using namespace std;

class GeneralException : public exception 
{
private:
	string errorMessage_;
public:
	GeneralException(string message);
	const char* what() const noexcept override;
};

#endif // _GENERALEXCEPTION_H_