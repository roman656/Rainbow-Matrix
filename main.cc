#include "MainController.h"

int main(int argc, char* argv[])
{
	int colorsArray[] = {
		COLOR_GREEN,	COLOR_BLACK,
		COLOR_RED,	    COLOR_BLACK,
		COLOR_BLUE, 	COLOR_BLACK,
		COLOR_CYAN,  	COLOR_BLACK,
		COLOR_YELLOW,	COLOR_BLACK,
		COLOR_MAGENTA,	COLOR_BLACK,
		COLOR_WHITE,	COLOR_BLACK 
	};
	MainController controller;
	controller.readParameters();
	controller.ncursesStart(0,1,0,1,7,colorsArray);
	controller.drawingStart();
}