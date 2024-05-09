#include "Socket.hpp"

void Socket::Init() {addrLen = sizeof(addr);}

void Socket::makeServerSocket(char *port)
{
	struct sockaddr_in servAddr;

	// 서버 소켓 생성 (IPv4, TCP/IP)
	servSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (servSocket == -1) throw std::exception("socket() error");

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(inet_addr(port));
	servAddr.sin_addr = inet_addr(INADDR_ANY);

	// 소켓 바인드
	if (bind(servSocket, static_cast<struct sockaddr *>(&servAddr), sizeof(servAddr)) == -1)
		throw std::exception("bind() error");

	// 듣기 소켓으로 변경
	if (listen(servSocket, 10) == -1)
		throw std::exception("listen() error")
}

int Socket::makeClientSocket()
{
	int ClientSocket;

	// 클라이언트와 연결을 위한 소켓 생성
	ClientSocket = accept(servSocket, static_cast<sockaddr *>(&addr), &addrlen);
	if (ClientSocket == -1)
		throw std::exception("accept() error");
	
	return (ClientSocket);
}