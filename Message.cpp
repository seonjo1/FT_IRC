#include "Message.hpp"

Message::Message(int fd) : fd(fd), endFlag(false), errFlag(false) {};

Message::~Message() {};

void Message::fillMsg()
{
	int readNum;
	char tmp[BUFFER_SIZE];

	while (1)
	{
		// 메시지 수신
		readNum = recv(fd, &tmp, BUFFER_SIZE, 0);
		if (readNum == -1)
		{
			// errno 가 EAGAIN 이나 EWOULDBLOCK 인 경우 recv() 에러가 아닌
			// 비동기 소켓이 읽을 내용이 없어서 block 되지 않고 -1 반환 (따라서 오류가 아님)
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break;

			// 위 if 문에 걸리지 않는 경우는 recv() 에러
			errFlag = true;
			break;
		}
		else if (readNum == 0)
		{
			// 클라이언트와 연결 종료 (eof)
			endFlag = true;
			break;
		}
		// 버퍼에 메시지 추가
		buf += tmp;
	}
}


bool Message::isError()
{
	// 에러 플래그 반환
	return (errFlag);
}

bool Message::isDisconnected()
{
	// 연결이 끊겼는지 나타내는 플래그 반환
	return (endFlag);
}

bool Message::isComplete()
{
	while (1)
	{
		// '\n' 위치 찾기
		NL = buf.find("\n");
		// '\n'이 발견되지 않았으면 명령어 완성 x
		if (NL == std::string::npos)
			return (false);
		// '\n'이 발견됐어도 앞에 '\r'가 없으면 올바른 명령어 형식이 아니므로 삭제하고 무시
		if (NL < 2 || buf[NL - 1] != '\r')
			buf.erase(0, NL + 1);
		else
			return (true); // "\r\n"이 존재하면 명령어 형식이 올바르므로 true 반환 
	}
}

std::string Message::getMsg()
{
	// 명령어 처음부터 "\r\n" 전까지로 저장
	std::string cmd = buf.substr(NL - 1);
	// buf에서 명령어 부분 제거
	buf.erase(0, NL + 1);
	return (cmd);
}
