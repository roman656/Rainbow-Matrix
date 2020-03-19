#ifndef _GENERALEXCEPTION_H_
#define _GENERALEXCEPTION_H_

#include <stdexcept>

///Класс основного исключения. Для наиболее общих случаев.
class GeneralException : public std::exception 
{
private:
	std::string errorMessage;	//Строка, содержащая сообщение, описывающее исключение.
public:
	///Конструктор.
	///Входные параметры:
	///\param std::string message - строка, содержащая сообщение, описывающее исключение.
	GeneralException(std::string message);	

	///Метод, возвращающий errorMessage.
	const char* what() const noexcept override;	
};

#endif // _GENERALEXCEPTION_H_