#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <cerrno>
# include <string>
# include <sys/socket.h>

# define BUFFER_SIZE 1024

class Message
{
public:
	Message(int fd);
	~Message();

	std::string getCmd(); // 버퍼에 있는 명령어
	void fillMsg(); // 클라이언트가 보낸 데이터 버퍼에 저장
	bool getErrFlag(); // errFlag 반환
	bool getEndFlag(); // endFlag 반환
	bool isComplete(); // 버퍼에 있는 명령어 완성 확인

private:
	int fd; // 데이터를 받는 fd
	int NL; // '\n' 위치
	std::string buf; // 메시지 저장 버퍼
	bool endFlag; // 클라이언트와 연결 종료 플래그
	bool errFlag; // recv 함수 에러 플래그
};

#endif
