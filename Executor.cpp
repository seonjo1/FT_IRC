#include "Executor.hpp"

Executor::Executor(char *password, std::vector<std::string> *nickList,
	std::map<int, Client> *clientList, Kqueue *kq)
	: password(password), nickList(nickList), clientList(clientList), kq(kq) {};

Executor::~Executor() {};


void Executor::execute(Client& client, std::string msg)
{
	std::vector<std::string> cmds = splitMsg(msg);

	if (!client.getPassFlag() || !client.getNickFlag() || !client.getUserFlag())
	{
		if (cmds[0] == "PASS")
			PASS(client, cmds);
		else if (cmds[0] == "NICK")
			NICK(client, cmds);
		else if (cmds[0] == "USER")
			USER(client, cmds);
		
		if (client.getPassFlag() && client.getNickFlag() && client.getUserFlag())

	}
	else
	{
		if (cmds[0] == "PASS")
			PASS(client, cmds);
		else if (cmds[0] == "NICK")
			NICK(client, cmds);
		else if (cmds[0] == "USER")
			USER(client, cmds);
	}
}

// nick이 유효한 nickname인지 확인
bool Executor::isInvalidNick(std::string& nick)
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

bool Executor::isNicknameInUse(std::string& nick)
{
	int size = nick.size();

	// 대문자를 소문자로 변환
	std::string lowercase;
	for (int i = 1; i < size; i++)
		lowercase += tolower(nick[i]);
	// nickname 대소문자 상관없이 중복 금지
	if (find(nickList->begin(), nickList->end(), lowercase) != nickList->end()) return (true);
	return (false);
}

void Executor::addNick(std::string& nick)
{
	int size = nick.size();

	// 대문자를 소문자로 변환
	std::string lowercase;
	for (int i = 1; i < size; i++)
		lowercase += tolower(nick[i]);
	// nickList에 추가
	nickList->push_back(lowercase);
}

void Executor::removeNick(std::string& nick)
{
	int size = nick.size();

	// 대문자를 소문자로 변환
	std::string lowercase;
	for (int i = 1; i < size; i++)
		lowercase += tolower(nick[i]);
	// channelList에서 nick 제거
	std::vector<std::string>::iterator iter = find(nickList->begin(), nickList->end(), lowercase);
	*iter = nick;
}

std::vector<std::string> Executor::splitMsg(std::string &msg)
{
	std::vector<std::string> cmds;
	std::stringstream ss(msg);

	std::string cmd;
	while (ss >> cmd)
		cmds.push_back(cmd);
	
	return (cmds);
}

void Executor::sendMsg(int fd, std::string msg)
{
	send(fd, msg.c_str(), msg.size(), 0);
}
