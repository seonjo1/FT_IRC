#include "../../include/utils/Message.hpp"

Message::Message(int fd) : fd(fd), endFlag(false), errFlag(false) {};

Message::~Message() {};

void Message::fillMsg()
{
	int readNum;
	char tmp[BUFFER_SIZE];

	if (buf.size() > MAX_BUFFER_SIZE)
		return ;
	// 메시지 수신
	readNum = recv(fd, &tmp, BUFFER_SIZE, 0);
	if (readNum == -1)
	{
		// recv() 에러
		errFlag = true;
		return ;
	}
	else if (readNum == 0)
	{
		// 클라이언트와 연결 종료 (eof)
		endFlag = true;
		return ;
	}
	// 버퍼에 메시지 추가
	buf += std::string(tmp, readNum);
}


bool Message::getErrFlag()
{
	// recv 함수 에러 여부를 나타내는 플래그 반환
	return (errFlag);
}

bool Message::getEndFlag()
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
		if (NL == std::string::npos || NL >= 512)
		{
			// 만약 512 길이에도 '\n'이 없다면 앞의 512 제거
			if (buf.size() > 512)
			{
				buf = buf.substr(512);
				continue ;
			}
			return (false);
		}
		// '\n'이 발견됐어도 앞에 '\r'가 없으면 올바른 명령어 형식이 아니므로 삭제하고 무시
		if (NL < 2 || buf[NL - 1] != '\r')
			buf.erase(0, NL + 1);
		else
			return (true); // "\r\n"이 존재하면 명령어 형식이 올바르므로 true 반환 
	}
}

std::string Message::getCmd()
{
	// 명령어 처음부터 "\r\n" 전까지로 저장
	std::string cmd = buf.substr(0, NL - 1);
	// buf에서 명령어 부분 제거
	buf.erase(0, NL + 1);
	return (cmd);
}
