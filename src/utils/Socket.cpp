#include "../../include/utils/Socket.hpp"
#include "../../include/core/Server.hpp"

int Socket::servSocket = 0;
struct sockaddr_in Socket::addr;
socklen_t Socket::addrLen = sizeof(addr);

void Socket::makeServerSocket(char *port)
{
	// 서버 소켓 생성 (IPv4, TCP/IP)
	servSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (servSocket == -1) throw std::runtime_error("socket() error");

	// 서버 소켓 정보 구조체에 저장
	struct sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(socket_atoi(port));
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

	// kq에 등록
	Kqueue& kq = Server::getKq();
	kq.addSocket(servSocket);
}

int Socket::makeClientSocket()
{
	// 클라이언트와 통신을 위한 소켓 생성
	int clientSocket = accept(servSocket, reinterpret_cast<sockaddr *>(&addr), &addrLen);
	if (clientSocket == -1)
		throw std::runtime_error("accept() error");

	// 소켓 비동기화
	int flag = fcntl(clientSocket, F_GETFL);
	fcntl(clientSocket, F_SETFL, flag | O_NONBLOCK);

	// kq에 등록
	Kqueue& kq = Server::getKq();
	kq.addSocket(clientSocket);

	return (clientSocket);
}