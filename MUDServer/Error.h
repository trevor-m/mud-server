#ifndef ERRORCLASS_H
#define ERRORCLASS_H

#include <string>
#include <iostream>
#include <algorithm>

class Error
{
private:
	static const int RTN_EXITPROMPT = 1; //the return value used for exit()
	static void ExitPrompt(); //prompts user to press any key to quit, then quits
	static std::string ToLocalFile(std::string filename); //cuts off the path for the string returned by __FILE__
public:
	static void MakeError(std::string message, std::string file, int line); //displays an error message and quits the program
};

#define MAKE_ERROR(x) Error::MakeError(x, __FILE__, __LINE__) //displays an error message and quits the program

#endif