#include "Server.hpp"

Server::Server(char *port, char *password)
	: executor(password, &nickList)
{
	Socket::makeServerSocket(port);   // 서버 소켓 생성
	kq.addSocket(Socket::servSocket); // kq에 등록
}

Server::~Server() {};

void Server::removeSocket(int socket)
{
	clientList.erase(socket); // 클라이언트 배열에서 제거
	kq.removeSocket(socket); // kq에서 제거
}

void Server::receiveClientRequest(int fd)
{
	if (fd == Socket::servSocket)
	{
		// 서버에 들어온 클라이언트 연결 요청
		int clientSocket = Socket::makeClientSocket(); // 연결 소켓 생성
		kq.addSocket(clientSocket); // kq에 등록
		clientList.insert(std::make_pair(clientSocket, Client(clientSocket, &nickList))); // 클라이언트 배열에 추가
	}
	else
	{
		// 서버에 연결되어있는 클라이언트에게 온 메시지 확인
		Client client = clientList.find(fd)->second; // fd에 맞는 클라이언트 찾기
		client.receiveMsg(); // 클라이언트가 보낸 메시지 받기
		while (client.isCmdComplete()) // 완성된 명령어가 있으면 실행
			executor.execute(client.getCmd());
		if (client.isDisconnected()) // eof가 들어온 경우 소켓 연결 종료
			removeSocket(fd);
	}
}

void Server::run()
{
	while (1)
	{
		// event 발생 개수 파악
		int eventCnt = kq.event();

		// event 발생한 만큼 Loop
		for (int idx = 0; idx < eventCnt; idx++)
		{
			// Event 발생에 오류 확인
			if (kq.isNotEventError(idx))
				receiveClientRequest(kq.getFd(idx)); // 클라이언트 요청 받기
			else
				throw std::runtime_error("kevent() EV_ERROR flag"); // 예외 던지기
		}
	}
}