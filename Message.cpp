#include "Message.hpp"

Message::Message(int fd) : fd(fd), endFlag(false), errFlag(false) {};

Message::~Message() {};

void Message::fillMsg()
{
	int readNum;
	char tmp;

	while (1)
	{
		// 메시지 수신
		readNum = recv(fd, &tmp, 1, 0);
		if (readNum == -1)
		{
			// errno 가 EAGAIN 이나 EWOULDBLOCK 인 경우 recv() 에러가 아닌
			// 비동기 소켓이 읽을 내용이 없어서 block 되지 않고 -1 반환 (따라서 오류가 아님)
			if (errno == EAGAIN || errno == EWOULDBLOCK) break;

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
		else if (tmp == '\n') // 메시지 끝까지 읽었으니 종료
			break;
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
	// 마지막이 '\r'으로 끝난 버퍼는 명령어가 완성됐다는 뜻!!
	if (buf.size() != 0 && buf[buf.size() - 1] == '\r')
		return (true);
	// 그게 아니면 아직 명령어가 덜 도착했으므로 다시 대기
	return (false);
}

std::string Message::getMsg()
{
	// 가지고 있던 메시지를 return 하면서 버퍼 초기화
	std::string tmp(buf);
	buf = "";
	return (tmp);
}
