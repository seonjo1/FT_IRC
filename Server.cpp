#include "Server.hpp"

Server::Server(char *port, char *password)
	: password(password)
{
	Socket::makeServerSocket(port);
	kq.addSocket(Socket::servSocket);
}

Server::~Server() {};

void Server::removeSocket(int socket)
{
	clientList.erase(socket);
	kq.removeSocket(socket);
	close(socket);
}

void Server::receiveClientRequest(int fd)
{
	if (fd == Socket::servSocket)
	{
		// 서버에 들어온 클라이언트 연결 요청
		int clientSocket = Socket::makeClientSocket();
		kq.addSocket(clientSocket);
		clientList.insert(std::make_pair(clientSocket, Client(clientSocket)));
	}
	else
	{
		// 서버에 연결되어있는 클라이언트에게 온 메시지 확인
		Client client = clientList.find(fd)->second;
		client.fillMsg();
		if (client.isCmdComplete())
			Executor::execute(client.getMsg(), password);
		if (client.isDisconnected())
			removeSocket(fd);
	}
}

void Server::run()
{
	while (1)
	{
		int eventCnt = kq.event();
		for (int idx = 0; idx < eventCnt; idx++)
		{
			if (kq.isEventTriggered(idx))
				receiveClientRequest(kq.getFd(idx));
			else
				throw std::runtime_error("kevent() EV_ERROR flag");
		}
	}
}