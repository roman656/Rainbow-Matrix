#ifndef _RANDOMSYMBOL_H_
#define _RANDOMSYMBOL_H_

#include <ncurses.h>
#include <cstdlib>

#define MY_ATTRIBUTES A_BOLD   	//Дополнительные атрибуты символа. Значение по умолчанию: A_NORMAL.
#define DEFAULT_COLOR_PAIR 1	//Цветовая пара (текст/фон) по умолчанию - 1я.

///Класс случайного символа.
class RandomSymbol
{
private:
	char symbol;		//Символ.
	int attribute;		//Атрибуты символа.
public:
	///Конструктор.
	///Входные параметры:
	///\param bool isPolychromy - флаг: может ли символ иметь случайный цвет. Если false - символ будет иметь цвет, заданный по умолчанию (DEFAULT_COLOR_PAIR). 
	RandomSymbol(bool isPolychromy);

	///Метод, печатающий по заданным координатам на экране символ. 
	///Входные параметры:
	///\param short yCoordinate - координата выводимого символа по оси OY;
	///\param short xCoordinate - координата выводимого символа по оси OX.
	void printRandomSymbol(short yCoordinate, short xCoordinate);
};

#endif // _RANDOMSYMBOL_H_