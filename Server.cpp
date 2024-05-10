#include "Server.hpp"

Server::Server(char *port, char *password)
	: password(password)
{
	Socket::makeServerSocket(port);
	kq.socketAdd(Socket::servSocket);
}

Server::~Server() {};

void Server::receiveClientRequest(int fd)
{
	if (fd == Socket::servSocket)
	{
		int clientSocket = Socket::makeClientSocket();
		kq.socketAdd(clientSocket);
		clientList.insert({clientSocket, Client()});
	}
	else
	{
		
	}
}

void Server::run()
{
	while (1)
	{
		int eventNum = kq.event();
		for (int idx = 0; idx < eventNum; idx++)
		{
			if (kq.flagCheck(idx))
				receiveClientRequest(kq.getFd(idx));
			else
				throw std::runtime_error("kevent() EV_ERROR flag");
		}
	}
}