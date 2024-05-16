#include "Server.hpp"

Kqueue kq;
std::vector<std::string> nickList;
std::map<int, Client> clientList;
std::map<std::string, Channel> channelList; 

Server::Server(char *port, char *password)
	: executor(password)
{
	// 서버 소켓 생성
	Socket::makeServerSocket(port);

	// hostname 저장
	char hostname[256];
	if (gethostname(hostname, sizeof(hostname)) == -1)
		throw std::runtime_error("gethostname() error");
	// hostentry 저장
	struct hostent* hostEntry = gethostbyname(hostname);
	if (hostEntry == NULL)
		throw std::runtime_error("gethostbyname() error");
	// 서버 IP 저장
	IP = inet_ntoa(*((struct in_addr*)hostEntry->h_addr_list[0]));
}

Server::~Server() {};

void Server::receiveClientRequest(int fd)
{
	if (fd == Socket::servSocket)
	{
		// 서버에 들어온 클라이언트 연결 요청
		int clientSocket = Socket::makeClientSocket(); // 연결 소켓 생성
		clientList.insert(std::make_pair(clientSocket, Client(clientSocket))); // 클라이언트 배열에 추가
	}
	else
	{
		// 서버에 연결되어있는 클라이언트에게 온 메시지 확인
		Client client = clientList.find(fd)->second; // fd에 맞는 클라이언트 찾기
		client.receiveMsg(); // 클라이언트가 보낸 메시지 받기
		while (client.isCmdComplete()) // 완성된 명령어가 있으면 실행
			executor.execute(client, client.getCmd());
		if (client.isDisconnected()) // eof가 들어온 경우 소켓 연결 종료
			clientList.erase(fd); // 클라이언트 배열에서 제거
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