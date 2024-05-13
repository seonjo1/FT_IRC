#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Message.hpp"

class Client
{
public:
	Client(int fd);
	~Client();

	void receiveMsg(); // 클라이언트가 보낸 데이터를 받음
	bool isDisconnected(); // 클라이언트와 연결 여부 확인
	bool isCmdComplete(); // 버퍼에 있는 명령어 완성 여부 확인
	std::string getCmd(); // 버퍼에 있는 명령어 반환

private:
	Message msg; // 클라이언트와 통신할때 사용할 객체
};

#endif
