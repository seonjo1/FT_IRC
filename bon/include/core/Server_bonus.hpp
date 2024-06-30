#ifndef SERVER_BONUS_HPP
# define SERVER_BONUS_HPP

# include <netdb.h>

# include "Channel_bonus.hpp"
# include "Executor_bonus.hpp"
# include "../utils/Socket_bonus.hpp"

class Server
{
public:
	Server(char *port, char *password);
	~Server();

	void run(); // 서버 실행
	
	static Kqueue& getKq(); // kqueue 관리 객체
	static std::vector<std::string>& getNickList(); // 닉네임 배열
	static std::map<int, Client>& getClientList(); // 클라이언트 배열
	static std::map<std::string, Channel>& getChannelList(); // 채널 리스트
	static std::string getIP(); // 서버 IP

private:
	Executor executor; // 명령어 실행기

	// void print_result(); // 디버깅 함수
	void receiveClientRequest(int fd); // 클라이언트 요청 받는 함수
	void sendMsgToClients(); // 클라이언트들에게 sendBuf에 있는 메시지 전송
};

#endif