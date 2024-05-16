#include "Server.hpp"
#include "Channel.hpp"

Channel::Channel(std::string& channelName)
	: channelName(channelName)
{
	timeChannelWasCreated = time(0);
}

Channel::~Channel() {};

 // channel 이름 유효성 검사
bool Channel::isInvalidChannelName(std::string& channel)
{
	// 규칙 1. 길이는 #포함 64까지
	if (channel.size() > 64)
		return (true);
	// 규칙 2. 공백(space) 콤마(,) unprintable 포함 x
	for (int i = 0; i < channel.size(); i++)
	{
		if (channel[i] == ' ' || channel[i] == ',' || !isprint(channel[i]))
			return (true);
	}
	return (false);
}

// channel이 존재하는지 확인
bool Channel::isChannelInUse(std::string& channel)
{
	for (int i = 0; i < channel.size(); i++)
		channel[i] = tolower(channel[i]);
	std::map<std::string, Channel>::iterator iter = Server::channelList.begin();
	for (; iter != Server::channelList.end(); iter++)
	{
		std::string name = iter->second.getName();
		for (int i = 0; i < name.size(); i++)
			name[i] = tolower(name[i]);
		if (name == channel)
			return (true);
	}
	return (false);
}

// channel에 참가
void Channel::addNickInChannel(Client& client)
{
	if (joinList.size() == 0 && opList.size() == 0)
		opList.push_back(client);
	else
		joinList.push_back(client);
	std::vector<std::string>::iterator iter = find(inviteList.begin(), inviteList.end(), client.getNick());
	if (iter != inviteList.end())
		inviteList.erase(iter);
}

// channel 나가기
void Channel::removeNickInChannel(Client& client)
{
	std::vector<Client>::iterator iter = find(joinList.begin(), joinList.end(), client);
	if (iter != joinList.end())
		joinList.erase(iter);
	
	iter = find(opList.begin(), opList.end(), client);
	if (iter != opList.end())
		opList.erase(iter);
}

// channel nick 변경
void Channel::changeNickInChannel(Client& client, std::string& newNick)
{
	std::vector<Client>::iterator iter = find(joinList.begin(), joinList.end(), client);
	if (iter != joinList.end())
		iter->setNick(newNick);
	
	iter = find(opList.begin(), opList.end(), client);
	if (iter != opList.end())
		iter->setNick(newNick);

	std::vector<std::string>::iterator inviteIter = find(inviteList.begin(), inviteList.end(), client.getNick());
	if (inviteIter != inviteList.end())
		*inviteIter = newNick;
}

// 채널에 메시지 전송
void Channel::sendToClients(std::string& msg)
{
	std::vector<Client>::iterator iter = opList.begin();
	for (; iter != opList.begin(); iter++)
		iter->sendMsg(msg);
	iter = joinList.begin();
	for (; iter != joinList.begin(); iter++)
		iter->sendMsg(msg);
}

// 채널에 클라이언트 있나 확인
bool Channel::doesClientExist(std::string& nick)
{
	std::vector<Client>::iterator iter = opList.begin();
	for (; iter != opList.begin(); iter++)
	{
		if (iter->getNick() == nick)
			return (true);
	}
	iter = joinList.begin();
	for (; iter != joinList.begin(); iter++)
	{
		if (iter->getNick() == nick)
			return (true);
	}
	return (false);
}

// 채널 삭제 (채널리스트에서 삭제)
void Channel::removeChannel(std::string& channel)
{
	for (int i = 0; i < channel.size(); i++)
		channel[i] = tolower(channel[i]);
	std::map<std::string, Channel>::iterator iter = Server::channelList.begin();
	for (; iter != Server::channelList.end(); iter++)
	{
		std::string name = iter->second.getName();
		for (int i = 0; i < name.size(); i++)
			name[i] = tolower(name[i]);
		if (name == channel)
			Server::channelList.erase(iter);
	}
}

// 채널 리스트에서 채널 찾아주는 함수
Channel& Channel::findChannel(std::string& channel)
{
	for (int i = 0; i < channel.size(); i++)
		channel[i] = tolower(channel[i]);
	std::map<std::string, Channel>::iterator iter = Server::channelList.begin();
	for (; iter != Server::channelList.end(); iter++)
	{
		std::string name = iter->second.getName();
		for (int i = 0; i < name.size(); i++)
			name[i] = tolower(name[i]);
		if (name == channel)
			return (iter->second);
	}
}

// getter
std::string Channel::getName()
{
	return (channelName);
}