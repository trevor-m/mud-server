#include "Server.h"
#include <iostream>


/*int AcceptThread(void* data)
{
//NEED TO ADD MUTEXS/LOCKING
	Server* serverPtr = (Server*)data;
	while(1)
	{
		//search for connecting clients
		serverPtr->OnClientConnect();
	}
	return 0;
}*/

int main()
{
	Server* server = new Server();
	server->Host();
	delete server;
	return 0;
}