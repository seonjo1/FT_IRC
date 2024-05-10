#ifndef SERVER_HPP
# define SERVER_HPP

# include <map>
# include <unistd.h>
# include "Socket.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "Kqueue.hpp"
# include "Executor.hpp"

class Server
{
public:
	Server(char *port, char *password);
	~Server();

	Kqueue kq; // kqueue 관리 객체
	std::map<int, Client> clientList; // 클라이언트 배열
	// std::map<std::string, Channel> channelList; // 채널 배열

	void run(); // 서버 실행

	void receiveClientRequest(int fd); // 클라이언트 요청 받는 함수
	void removeSocket(int fd); // 연결 종료된 소켓 정리 함수

private:
	char *password;
};

#endif