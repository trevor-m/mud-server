#include "Error.h"

void Error::ExitPrompt()
{
	std::cout << "Press any key to quit... ";
	//wait for user input
	getchar();
	exit(Error::RTN_EXITPROMPT);
}

std::string Error::ToLocalFile(std::string filename)
{
	//this will hold the new name
	std::string localname;

	//start at the end of the string, loop until a '/' or '\' is found
	for(int i = filename.length()-1; i >= 0; i--)
	{
		if(filename[i] == '/' || filename[i] == '\\')
		{
			//name is finished, but backwards
			//reverse it
			std::reverse(localname.begin(), localname.end());
			return localname;
		}
		//add the character to the local name
		localname += filename[i];
	}

	//found no \ or /
	return filename;
}

void Error::MakeError(std::string message, std::string file, int line)
{
	//display error message
	std::cout << std::endl
		<< "Error occured in file:" << Error::ToLocalFile(file) << " line:" << line <<  " \"" << message << "\"" << std::endl;
	//display exit prompt
	Error::ExitPrompt();
}