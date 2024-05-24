#include "../../include/core/Client.hpp"
#include "../../include/core/Server.hpp"
#include "../../include/core/Channel.hpp"

// #include <iostream>

// 생성자
Client::Client(int fd)
	: msg(fd), fd(fd), origin(false), passFlag(false), nickFlag(false),
		userFlag(false), quitFlag(false), connectFlag(false), registerFlag(false) {};

// 소멸자
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

/* 서버에서 소켓 메시지 받을때 사용하는 함수들 */

// 클라이언트가 보낸 데이터를 받음
void Client::receiveMsg()
{
	// 메시지에 클라이언트가 보낸 내용을 저장
	msg.fillMsg();
	// 메시지를 저장하다 발생한 에러 감지
	if (msg.getErrFlag())
		throw std::runtime_error("recv() error");
}

// 클라이언트와 연결 여부 확인
bool Client::isDisconnected()
{
	// 연결이 끊겼는지 확인
	if (msg.getEndFlag() || quitFlag)
		return (true);
	return (false);
}

// 버퍼에 있는 명령어 완성 여부 확인
bool Client::isCmdComplete()
{
	// 메시지 버퍼에 있는 명령어가 완성됐는지 확인
	if (msg.isComplete())
		return (true);
	return (false);
}

// 버퍼에 있는 명령어 반환
std::string Client::getCmd()
{
	// 메시지 반환
	return (msg.getCmd());
}

/* 플래그 반환 함수 */

// 등록 여부 확인
bool Client::isRegistered()
{
	return (registerFlag);
}

/* <nickname 함수들> */

// 유효한 nickname인지 확인
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

// 이미 사용중인 nickname인지 확인
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

// nickname NickList에 추가하고 클라이언트 nickname 수정
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

// NickList과 ChannelList에서 닉네임 삭제 
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

// NickList와 참여한 채널들에 nickname 변경
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
	std::set<Client*> set;
	set.insert(this);
	std::vector<Channel*>::iterator channelIter = joinedChannels.begin();
	for (; channelIter != joinedChannels.end(); channelIter++)
		(*channelIter)->changeNickInChannel(*this, nick, set);
	
	// nickname 변경 메시지 만들기
	std::string msg = ServerMsg::NICKCHANGE(nickname, data.hostname, data.servername, nick);

	// set에 있는 같은 채널에 있는 client들의 fd에 메시지 전송
	std::set<Client*>::iterator setIter = set.begin();
	for (; setIter != set.end(); setIter++)
		(*setIter)->addToSendBuf(msg);

	// nickname 변경
	nickname = nick;
}


/* 클라이언트에게 보낼 sendBuf 관리 함수 */

// sendBuf에 있는 내용 클라이언트에게 전달하고 sendBuf 초기화
void Client::sendMsg()
{
	if (sendBuf.size() > 0)
	{
		// 보낸 메시지 확인용
		// std::cout << "send to client : " << sendBuf;
		const char *msg = sendBuf.c_str();
		send(fd, msg, sendBuf.size(), 0);
		sendBuf = "";
	}
}

// sendBuf에 메시지 추가
void Client::addToSendBuf(std::string msg)
{
	sendBuf += msg;
}


/* channel 관련 함수 */

// 클라이언트가 참여한 채널리스트에 해당 채널 추가 
void Client::addJoinedChannels(Channel* channel)
{
	joinedChannels.push_back(channel);
}

// 클라이언트가 참여한 채널리스트에 해당 채널 삭제
void Client::removeJoinedChannels(Channel* channel)
{
	joinedChannels.erase(find(joinedChannels.begin(), joinedChannels.end(), channel));
}

// 클라이언트가 해당 채널에 참여했는지 확인
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

/* getter */

// nick과 일치하는 Client 반환
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

// 클라이언트 fd 반환
int Client::getFd()
{
	return (fd);
}

// 클라이언트 PassFlag 전달
bool Client::getPassFlag()
{
	return (passFlag);
}

// 클라이언트 NickFlag 전달
bool Client::getNickFlag()
{
	return (nickFlag);
}

// 클라이언트 UserFlag 전달
bool Client::getUserFlag()
{
	return (userFlag);
}

// 클라이언트 QuitFlag 전달
bool Client::getQuitFlag()
{
	return (quitFlag);
}

// 클라이언트 ConnectFlag 전달
bool Client::getConnectFlag()
{
	return (connectFlag);
}

// 클라이언트 hostname 전달
std::string Client::getHostName()
{
	return (data.hostname);
}

// 클라이언트 servername 전달
std::string Client::getServerName()
{
	return (data.servername);
}

// 클라이언트 nickname 전달
std::string& Client::getNick()
{
	return (nickname);
}

// 클라이언트 joinedChannels 전달
std::vector<Channel*>& Client::getJoinedChannels()
{
	return (joinedChannels);
}


/* setter */

// PassFlag 변경
void Client::setPassFlag(bool sign)
{
	passFlag = sign;
	if (passFlag && nickFlag && userFlag)
		registerFlag = true;
}

// NickFlag 변경
void Client::setNickFlag(bool sign)
{
	nickFlag = sign;
	if (passFlag && nickFlag && userFlag)
		registerFlag = true;
}

// UserFlag 변경
void Client::setUserFlag(bool sign)
{
	userFlag = sign;
	if (passFlag && nickFlag && userFlag)
		registerFlag = true;
}

// Origin 변경
void Client::setOrigin(bool sign)
{
	origin = sign;
}

// QuitFlag 변경
void Client::setQuitFlag(bool sign)
{
	quitFlag = sign;
}

// connectFlag 변경
void Client::setConnectFlag(bool sign)
{
	connectFlag = sign;
}

// nickname 변경
void Client::setNick(std::string& newNick)
{
	nickname = newNick;
}

// data 변경
void Client::setData(std::string& username, std::string& hostname,
				 std::string& servername, std::string& realname)
{
	data.username = username;
	data.hostname = hostname;
	data.servername = servername;
	data.realname = realname;
}

/* 연산자 오버로딩 */

bool Client::operator==(const Client& rhs)
{
	if (this->nickname == rhs.nickname)
		return (true);
	return (false);
}
