#include "Client.hpp"

Client::Client(int fd) : msg(fd) {};

Client::~Client() {};

void Client::fillMsg()
{
	msg.fillMsg();
	if (msg.isError())
		throw std::runtime_error("recv() error");
}

bool Client::isDisconnected()
{
	if (msg.isDisconnected())
		return (true);
	return (false);
}

bool Client::isCmdComplete()
{
	if (msg.isComplete())
		return (true);
	return (false);
}

std::string Client::getMsg()
{
	return (msg.getMsg());
}
