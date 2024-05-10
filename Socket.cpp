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

	// 소켓 비동기화
	int flag = fcntl(servSocket, F_GETFL);
	fcntl(servSocket, F_SETFL, flag | O_NONBLOCK);
}

int Socket::makeClientSocket()
{
	int clientSocket;

	// 클라이언트와 연결을 위한 소켓 생성
	clientSocket = accept(servSocket, reinterpret_cast<sockaddr *>(&addr), &addrLen);
	if (clientSocket == -1)
		throw std::runtime_error("accept() error");

	// 소켓 비동기화
	int flag = fcntl(clientSocket, F_GETFL);
	fcntl(clientSocket, F_SETFL, flag | O_NONBLOCK);
	// 비동기화 구분법 (read 오류가 아닌 읽을 게 없는 것) 
	// read == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)

	return (clientSocket);
}