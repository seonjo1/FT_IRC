#include "Message.hpp"

Message::Message(int fd) : fd(fd), endFlag(false), errFlag(false) {};

Message::~Message() {};

void Message::fillMsg()
{
	int readNum;
	char tmp;

	while (1)
	{
		readNum = recv(fd, &tmp, 1, 0);
		if (readNum == -1)
		{
			if (errno != EAGAIN && errno != EWOULDBLOCK)
				errFlag = true;
			break;
		}
		else if (readNum == 0)
		{
			endFlag = true;
			break;
		}
		else if (tmp == '\n')
			break;
		buf += tmp;
	}
}


bool Message::isError()
{
	return (errFlag);
}

bool Message::isDisconnected()
{
	return (endFlag);
}

bool Message::isComplete()
{
	if (buf.size() != 0 && buf[buf.size() - 1] == '\r')
		return (true);
	return (false);
}

std::string Message::getMsg()
{
	std::string tmp(buf);
	buf = "";
	return (tmp);
}
