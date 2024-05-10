#include "Client.hpp"

Client::Client(int fd) : msg(fd) {};

Client::~Client() {};

void Client::fillMsg()
{
	// 메시지에 클라이언트가 보낸 내용을 저장
	msg.fillMsg();
	// 메시지를 저장하다 발생한 에러 감지
	if (msg.isError())
		throw std::runtime_error("recv() error");
}

bool Client::isDisconnected()
{
	// 연결이 끊겼는지 확인
	if (msg.isDisconnected())
		return (true);
	return (false);
}

bool Client::isCmdComplete()
{
	// 메시지 버퍼에 있는 명령어가 완성됐는지 확인
	if (msg.isComplete())
		return (true);
	return (false);
}

std::string Client::getMsg()
{
	// 메시지 반환
	return (msg.getMsg());
}
