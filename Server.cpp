#include "Server.hpp"

Server::Server(char *port, char *password)
	: password(password)
{
	Socket::makeServerSocket(port);
}

Server::~Server() {};

void Server::run()
{
	while (1)
	{
		break;
	}
}