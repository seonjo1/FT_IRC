#ifndef SERVER_HPP
# define SERVER_HPP

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

	void run(); // 서버 실행

private:
	Kqueue kq; // kqueue 관리 객체
	Executor executor; // 명령어 실행기
	char *password; // server password
	std::map<int, Client> clientList; // 클라이언트 배열
	std::vector<std::string> nickList; // 닉네임 배열

	void receiveClientRequest(int fd); // 클라이언트 요청 받는 함수
	void removeSocket(int fd); // 연결 종료된 소켓 정리 함수
};

#endif