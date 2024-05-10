#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Message.hpp"

class Client
{
public:
	Client(int fd);
	~Client();

	void fillMsg(); // 클라이언트가 보낸 데이터를 메시지에 채우는 함수
	bool isDisconnected(); // 클라이언트와 연결 여부 확인
	bool isCmdComplete(); // 버퍼에 있는 명령어 완성 여부 확인
	std::string getMsg(); // 버퍼에 있는 메시지 반환

private:
	Message msg; // 클라이언트와 통신할때 사용할 객체
};

#endif
