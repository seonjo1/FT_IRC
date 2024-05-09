#include "Socket.hpp"

int Socket::servSocket = 0;
struct sockaddr_in Socket::addr;
socklen_t Socket::addrLen = sizeof(addr);

void Socket::makeServerSocket(char *port)
{
	struct sockaddr_in servAddr;

	// 서버 소켓 생성 (IPv4, TCP/IP)
	servSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (servSocket == -1) throw std::runtime_error("socket() error");

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(inet_addr(port));
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// 소켓 바인드
	if (bind(servSocket, reinterpret_cast<struct sockaddr *>(&servAddr), sizeof(servAddr)) == -1)
		throw std::runtime_error("bind() error");

	// 듣기 소켓으로 변경
	if (listen(servSocket, 10) == -1)
		throw std::runtime_error("listen() error");
}

int Socket::makeClientSocket()
{
	int ClientSocket;

	// 클라이언트와 연결을 위한 소켓 생성
	ClientSocket = accept(servSocket, reinterpret_cast<sockaddr *>(&addr), &addrLen);
	if (ClientSocket == -1)
		throw std::runtime_error("accept() error");
	
	return (ClientSocket);
}