#ifndef _MAINCONTROLLER_H_
#define _MAINCONTROLLER_H_

#include "UserInputException.h"
#include "Line.h"
#include "List.h"
#include "Explosion.h"
#include <iostream>
#include <sstream>

#define DEFAULT_POLYCHROMY true				//Значение флага многоцветности по умолчанию.
#define DEFAULT_SYMBOL_SPAWN_FREQUENCY 10		//Частота появления символов (скорость линий) по умолчанию.
#define DEFAULT_LINE_SPAWN_FREQUENCY 5			//Частота появления линий по умолчанию.
#define DEFAULT_LINE_SIZE 4 				//Размер линий по умолчанию.
#define DEFAULT_LINE_EXPLOSION_PROBABILITY 1		//Вероятность взрыва линии (%) по умолчанию.
#define DEFAULT_MIN_EXPLOSION_RADIUS 1			//Минимальный радиус взрыва линии по умолчанию.
#define DEFAULT_MAX_EXPLOSION_RADIUS 15			//Максимальный радиус взрыва линии по умолчанию.
#define DEFAULT_EXPLOSION_RADIAL_SPEED 10		//Радиальная скорость взрыва (частота появления символов) по умолчанию.

///Класс контроллера.
class MainController
{
private:
	bool isPolychromy;				//Флаг: использовать ли различные цвета.
	short lineSize;					//Размер линий (число символов в линии).
	short lineSpawnFrequency;			//Частота появления линий (число линий в еденицу времени).
	short symbolSpawnFrequency;			//Частота появления символов (число символов в еденицу времени).
	short lineExplosionProbability;			//Вероятность взрыва линии (%) [0;100].			
	short minExplosionRadius;			//Минимальный радиус взрыва линии.
	short maxExplosionRadius;			//Максимальный радиус взрыва линии.
	short explosionRadialSpeed;			//Радиальная скорость взрыва (частота появления символов).

	///Метод, считывающий целочисленный пользовательский ввод.
	///Входные параметры:
	///\param short maxValue - максимальное допустимое значение целочисленного пользовательского ввода;
	///\param short minValue - минимальное допустимое значение целочисленного пользовательского ввода;
	///\param short& parameter - ссылка на целочисленный параметр, в который будет записан обработанный особым образом пользовательский ввод.
	void readUserIntegerInput(short maxValue, short minValue, short& parameter);

	///Метод, считывающий символьный пользовательский ввод. 
	///Входные параметры:
	///\param bool& parameter - ссылка на флаг, в который будет записан обработанный особым образом пользовательский ввод.
	void readUserSymbolInput(bool& parameter);
public:
	///Конструктор.
	MainController();

	///Метод, получающий исходные данные от пользователя.
	void readParameters();

	///Метод, инициализирующий ncurses.
	///Входные параметры:
	///\param bool useCursor - флаг: необходим ли курсор;
	///\param bool useKeypad - флаг: необходима ли поддержка "горячих клавиш";
	///\param bool useEcho - флаг: необходимо ли подключить режим echo;
	///\param bool useColor - флаг: необходимо ли использовать цвета;
	///\param int nColorPairs - количество цветовых пар (текст/фон);
	///\param int* colorsArray - массив, содержащий цветовые пары в формате: {цвет текста 1й цветовой пары, цвет фона 1й цветовой пары, цвет текста 2й цветовой пары...}.
	void ncursesStart(bool useCursor, bool useKeypad, bool useEcho, bool useColor, int nColorPairs, int* colorsArray);

	///Метод, запускающий и поддерживающий процесс отрисовки "Матрицы".
	void drawingStart();
};

#endif // _MAINCONTROLLER_H_
