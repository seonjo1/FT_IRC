#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

#include <iostream>

Client::Client(int fd)
	: msg(fd), fd(fd), origin(false), passFlag(false), nickFlag(false),
		userFlag(false), quitFlag(false), connectFlag(false), registerFlag(false) {};

Client::~Client()
{
	if (origin)
	{
		Kqueue& kq = Server::getKq();
		// nickList에서 nickname제거
		if (nickFlag)
			removeNick();
		// kqueue에서 제거
			kq.removeSocket(fd); // kq에서 제거
		// 소켓 닫기
			close(fd);
	}
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
	if (msg.getEndFlag() || quitFlag)
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

bool Client::isRegistered()
{
	return (registerFlag);
}

// <nickname 함수들>
// nick이 유효한 nickname인지 확인
bool Client::isInvalidNick(std::string& nick)
{
	int size = nick.size();
	if (size > 30) return (true); // 규칙 1. 길이 최대 30
	if (!isalpha(nick[0])) return (true); // 규칙 2. 맨 앞은 알파벳으로 시작
	
	for (int i = 0; i < size; i++)
	{
		if (nick[i] == ' ') return (true); // 규칙 3. 공백 포함 x
		else if (!(isalpha(nick[i]) || isdigit(nick[i]) // 규칙 4. 숫자, 알파벳, '-', '_' 만으로 구성
			|| nick[i] == '-' || nick[i] == '_'))
			return (true);
	}
	return (false);
}

bool Client::isNicknameInUse(std::string& nick)
{
	std::vector<std::string>& nickList = Server::getNickList();

	int size = nick.size();
	// 대문자를 소문자로 변환
	std::string lowercase;
	for (int i = 0; i < size; i++)
		lowercase += tolower(nick[i]);
	
	// nickname 대소문자 상관없이 중복 금지
	if (find(nickList.begin(), nickList.end(), lowercase) != nickList.end()) return (true);
	return (false);
}

void Client::addNick(std::string nick)
{
	std::vector<std::string>& nickList = Server::getNickList();

	// nickname 갱신
	nickname = nick;

	int size = nick.size();

	// 대문자를 소문자로 변환
	std::string lowercase;
	for (int i = 0; i < size; i++)
		lowercase += tolower(nick[i]);
	
	// nickList에 추가
	nickList.push_back(lowercase);
}

void Client::removeNick()
{
	int size = nickname.size();
	if (size != 0)
	{
		std::vector<std::string>& nickList = Server::getNickList();

		// 대문자를 소문자로 변환
		std::string lowercase;
		for (int i = 0; i < size; i++)
			lowercase += tolower(nickname[i]);
		// nickList에서 nick 제거
		nickList.erase(find(nickList.begin(), nickList.end(), lowercase));
		// channelList에서 제거
		std::vector<Channel*>::iterator iter = joinedChannels.begin();
		for (; iter != joinedChannels.end(); iter++)
		{
			(*iter)->removeNickInChannel(*this);
			if ((*iter)->getSize() == 0)
				Channel::removeChannel((*iter)->getName());
		}
	}
}

void Client::changeNick(std::string nick)
{	
	std::vector<std::string>& nickList = Server::getNickList();

	// old nickname 소문자로 변환
	std::string oldLower;
	for (int i = 0; i < static_cast<int>(nickname.size()); i++)
		oldLower += tolower(nickname[i]);
	// new nickname 소문자로 변환
	std::string newLower;
	for (int i = 0; i < static_cast<int>(nick.size()); i++)
		newLower += tolower(nick[i]);

	// nickList에서 old nickname 찾기
	std::vector<std::string>::iterator nickIter = find(nickList.begin(), nickList.end(), oldLower);
	// old nickname 에서 new nickname 으로 변환
	*nickIter = newLower;


	// 같은 채널에 있는 nickname의 fd 들을 set에 저장
	std::set<int> set;
	set.insert(fd);
	std::vector<Channel*>::iterator channelIter = joinedChannels.begin();
	for (; channelIter != joinedChannels.end(); channelIter++)
		(*channelIter)->changeNickInChannel(*this, nick, set);
	
	// nickname 변경 메시지 만들기
	std::string msgStr = ServerMsg::NICKCHANGE(nickname, data.hostname, data.servername, nick);
	const char *msg = msgStr.c_str();
	
	// set에 있는 같은 채널에 있는 client들의 fd에 메시지 전송
	std::set<int>::iterator setIter = set.begin();
	for (; setIter != set.end(); setIter++)
		send(*setIter, msg, msgStr.size(), 0);

	// nickname 변경
	nickname = nick;
}

// 메시지 보내는 함수

void Client::sendMsg(std::string msg)
{
	std::cout << "send to client : " << msg;
	send(fd, msg.c_str(), msg.size(), 0);
}


// channel 함수
bool Client::isClientMemberOfChannel(std::string& channel)
{
	// channelList의 nick 변경
	std::vector<Channel*>::iterator iter = joinedChannels.begin();
	for (; iter != joinedChannels.end(); iter++)
	{
		if ((*iter)->getName() == channel)
			return (true);
	}
	return (false);
}

void Client::addJoinedChannels(Channel* channel)
{
	joinedChannels.push_back(channel);
}

void Client::removeJoinedChannels(Channel* channel)
{
	joinedChannels.erase(find(joinedChannels.begin(), joinedChannels.end(), channel));
}


// getter

Client& Client::getClient(std::string& nick)
{
	std::map<int, Client>& clientList = Server::getClientList();

	std::map<int, Client>::iterator iter = clientList.begin();
	for (; iter != clientList.end(); iter++)
	{
		if (iter->second.getNick() == nick)
			break;
	}
	return (iter->second);
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

std::string& Client::getNick()
{
	return (nickname);
}

int Client::getFd()
{
	return (fd);
}

std::string Client::getHostName()
{
	return (data.hostname);
}

std::string Client::getServerName()
{
	return (data.servername);
}

bool Client::getQuitFlag()
{
	return (quitFlag);
}

bool Client::getConnectFlag()
{
	return (connectFlag);
}

std::vector<Channel*>& Client::getJoinedChannels()
{
	return (joinedChannels);
}


// setter

void Client::setPassFlag(bool sign)
{
	passFlag = sign;
	if (passFlag && nickFlag && userFlag)
		registerFlag = true;
}

void Client::setNickFlag(bool sign)
{
	nickFlag = sign;
	if (passFlag && nickFlag && userFlag)
		registerFlag = true;
}

void Client::setUserFlag(bool sign)
{
	userFlag = sign;
	if (passFlag && nickFlag && userFlag)
		registerFlag = true;
}

void Client::setNick(std::string& newNick)
{
	nickname = newNick;
}

void Client::setData(std::string& username, std::string& hostname,
				 std::string& servername, std::string& realname)
{
	data.username = username;
	data.hostname = hostname;
	data.servername = servername;
	data.realname = realname;
}

void Client::setOrigin(bool sign)
{
	origin = sign;
}

void Client::setQuitFlag(bool sign)
{
	quitFlag = sign;
}

void Client::setConnectFlag(bool sign)
{
	connectFlag = sign;
}


// 연산자

bool Client::operator==(const Client& rhs)
{
	if (this->nickname == rhs.nickname)
		return (true);
	return (false);
}
