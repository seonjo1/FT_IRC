#ifndef SERVER_HPP
# define SERVER_HPP

# include "Socket.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "Executor.hpp"
# include <netdb.h>

class Server
{
public:
	Server(char *port, char *password);
	~Server();

	void run(); // 서버 실행
	
	static Kqueue kq; // kqueue 관리 객체
	static std::vector<std::string> nickList; // 닉네임 배열
	static std::map<int, Client> clientList; // 클라이언트 배열
	static std::map<std::string, Channel> channelList; // 채널 리스트
	static std::string IP; // 서버 IP

private:
	Executor executor; // 명령어 실행기
	char *password; // server password

	void receiveClientRequest(int fd); // 클라이언트 요청 받는 함수
};

#endif