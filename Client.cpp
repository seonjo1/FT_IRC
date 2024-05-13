#include "Client.hpp"
#include "Channel.hpp"

Client::Client(int fd, std::vector<std::string> *nickList)
	: fd(fd), msg(fd), nickList(nickList) {};

Client::~Client()
{
	// nickList에서 nickname제거
		int size = 0;
		std::string lowercase;
		// nickname의 대문자를 전부 소문자로 변경
		for (int i = 0; i < size; i++)
			lowercase += tolower(nickname[i]);
		// nickname을 nickList에서 찾아 제거
		nickList->erase(find(nickList->begin(), nickList->end(), lowercase));

	// channelList에서 nick 제거
		std::vector<Channel>::iterator iter = channelList.begin();
		for(; iter != channelList.end(); iter++)
			iter->removeNick(nickname);

	// 소켓 닫기
		close(fd);
}

void Client::receiveMsg()
{
	// 메시지에 클라이언트가 보낸 내용을 저장
	msg.fillMsg();
	// 메시지를 저장하다 발생한 에러 감지
	if (msg.getErrFlag())
		throw std::runtime_error("recv() error");
}

bool Client::isDisconnected()
{
	// 연결이 끊겼는지 확인
	if (msg.getEndFlag())
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

std::string Client::getCmd()
{
	// 메시지 반환
	return (msg.getCmd());
}

bool Client::getPassFlag()
{
	return (passFlag);
}

bool Client::getNickFlag()
{
	return (nickFlag);
}

bool Client::getUserFlag()
{
	return (userFlag);
}

void Client::setNick(std::string& newNick)
{
	nickname = newNick;
}

std::string& Client::getNick()
{
	return (nickname);
}

void Client::setData(std::string& username, std::string& hostname,
				 std::string& servername, std::string& realname)
{
	data.username = username;
	data.hostname = hostname;
	data.servername = servername;
	data.realname = realname;
}