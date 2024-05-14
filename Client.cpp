#include "Client.hpp"
#include "Channel.hpp"

Client::Client(int fd)
	: fd(fd), msg(fd) {};

Client::~Client()
{
	// nickList에서 nickname제거
		removeNick(nick);

	// channelList에서 nick 제거
		std::vector<Channel>::iterator iter = channelVec.begin();
		for(; iter != channelVec.end(); iter++)
			iter->removeNickInChannel(nickname);
	// kqueue에서 제거
		Server::kq.removeSocket(socket); // kq에서 제거
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

int Client::getFd()
{
	return (fd);
}

void Client::setPassFlag(bool sign)
{
	passFlag = sign;
}

void Client::setNickFlag(bool sign)
{
	nickFlag = sign;
}

void Client::setUserFlag(bool sign)
{
	userFlag = sign;
}


// <nickname 함수들>
// nick이 유효한 nickname인지 확인
bool Client::isInvalidNick(std::string& nick)
{
	int size = nick.size();
	if (size > 30) return (true); // 규칙 1. 길이 최대 30
	if (!isalpha(nick[0])) return (true); // 규칙 2. 맨 앞은 알파벳으로 시작
	
	for (int i = 1; i < size; i++)
	{
		if (nick[i] == ' ') return (true); // 규칙 3. 공백 포함 x
		else if (!isalpha(nick[i]) || !isdigit(nick[i]) // 규칙 4. 숫자, 알파벳, '-', '_' 만으로 구성
			|| nick[i] != '-' || nick[i] != '_')
			return (true);
	}
	return (false);
}

bool Client::isNicknameInUse(std::string& nick)
{
	int size = nick.size();

	// 대문자를 소문자로 변환
	std::string lowercase;
	for (int i = 1; i < size; i++)
		lowercase += tolower(nick[i]);
	// nickname 대소문자 상관없이 중복 금지
	if (find(Server::nickList.begin(), Server::nickList.end(), lowercase) != Server::nickList.end()) return (true);
	return (false);
}

void Client::addNickInNickList(std::string& nick)
{
	int size = nick.size();

	// 대문자를 소문자로 변환
	std::string lowercase;
	for (int i = 1; i < size; i++)
		lowercase += tolower(nick[i]);
	// nickList에 추가
	Server::nickList.push_back(lowercase);
}

void Client::removeNickInNickLIst(std::string& nick)
{
	int size = nick.size();

	// 대문자를 소문자로 변환
	std::string lowercase;
	for (int i = 1; i < size; i++)
		lowercase += tolower(nick[i]);
	// channelList에서 nick 제거
	Server::nickList.erase(find(Server::nickList.begin(), Server::nickList.end(), lowercase))
}

// 메시지 보내느 함수

void Client::sendMsg(std::string msg)
{
	send(fd, msg.c_str(), msg.size(), 0);
}
