#pragma once

#include <string>
#include <sstream>

enum Colors
{
	BLACK = 0,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	BROWN,
	LIGHTGREY,
	DARKGREY,
	LIGHTBLUE,
	LIGHTGREEN,
	LIGHTCYAN,
	LIGHTRED,
	LIGHTMAGENTA,
	YELLOW,
	WHITE
};

class Color
{
private:
public:
	static int ColorsToAnsi(Colors color);
	static void ChangeColor(std::string& string, Colors color);
	Color(void);
	~Color(void);
};

std::string IntToStr(int num);
int StrToInt(const std::string& str);