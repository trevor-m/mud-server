#include "Color.h"

Color::Color(void)
{
}

Color::~Color(void)
{
}

std::string IntToStr(int num)
{
	std::stringstream ss;
	ss << num;
	return ss.str();
}

int StrToInt(const std::string& str)
{
	std::stringstream ss(str);
	int ret;
	ss >> ret;
	return ret;
}

int Color::ColorsToAnsi(Colors color)
{
	switch(color)
	{
	case BLACK:
		//return 30;
	case RED:
		return 31;
	case LIGHTGREEN:
	case GREEN:
		return 32;
	case YELLOW:
		return 33;
	case BLUE:
		return 34;
	case MAGENTA:
		return 35;
	case LIGHTCYAN:
	case CYAN:
		return 36;
	case WHITE:
	default:
		return 0;
	}
}

void Color::ChangeColor(std::string& string, Colors color)
{
	string += "\x1b["; //start of escape equence
	string += IntToStr(Color::ColorsToAnsi(color)); //convert color to correct number
	string += "m"; //finish sequence
}
