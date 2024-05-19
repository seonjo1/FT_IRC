#include "Server.hpp"
#include "Channel.hpp"

Channel::Channel(std::string& channelName)
	: channelName(channelName), timeChannelWasCreated(time(0)), topicFlag(false) {};

Channel::~Channel() {};

// 채널에 참가
void Channel::joinChannel(Client& client, std::string& channelName, std::string param)
{
	if (isChannelInUse(channelName)) // 기존 채널이 있는 경우
	{
		// channel 찾기
		Channel& channel = findChannel(channelName);
		
		// key 모드인데 key가 틀린경우
		if (channel.isKeyMode() && channel.getKey() != param)
		{
			client.sendMsg(ServerMsg::BADCHANNELKEY(client.getNick(), channel.getName()));
			return ;
		}

		// invite 여부 확인
		if (channel.isInviteMode() && channel.isInvitedClient(client.getNick()))
		{
			client.sendMsg(ServerMsg::INVITEONLYCHAN(client.getNick(), channel.getName()));
			return ;
		}

		// 풀방 확인
		if (channel.isLimitMode() && channel.getLimit() <= channel.getSize())
		{
			client.sendMsg(ServerMsg::CHANNELISFULL(client.getNick(), channel.getName()));
			return ;
		}

		// 채널에 참여
		channel.addNickInChannel(client);

		// 토픽확인하고 참가자에게 토픽 메시지 전송
		if (channel.doesTopicExist())
		{
			client.sendMsg(ServerMsg::TOPIC(client.getNick(), channel.getName(), channel.getTopic()));
			client.sendMsg(ServerMsg::TOPICINFO(client.getNick(), channel.getName(),
												channel.getTopicWriter(), channel.getTopicTime()));
		}

		// 채널에 모든이들에게 join 메시지전송
		channel.sendToClients(ServerMsg::JOIN(client.getNick(), client.getHostName(),
											client.getServerName(), channel.getName()));
	}
	else // 채널이 없어서 만들어야하는 경우
	{
		Channel& channel = addChannel(channelName);

		// 채널에 참여
		channel.addNickInChannel(client);

		// join 메시지전송
		client.sendMsg(ServerMsg::JOIN(client.getNick(), client.getHostName(),
									 client.getServerName(), channel.getName()));
	
	}
}


 // channel 이름 유효성 검사
bool Channel::isInvalidChannelName(std::string& channel)
{
	// 규칙 1. 맨 앞의 char는 '#' or '&'
	if (channel[0] != '#' || channel[0] != '&')
		return (true);
	// 규칙 2. 길이는 #포함 64까지
	if (static_cast<int>(channel.size()) > 64)
		return (true);
	// 규칙 3. 공백(space) 콤마(,) unprintable 포함 x
	for (int i = 1; i < static_cast<int>(channel.size()); i++)
	{
		if (channel[i] == ' ' || channel[i] == ',' || !isprint(channel[i]))
			return (true);
	}
	return (false);
}

// channel이 존재하는지 확인
bool Channel::isChannelInUse(std::string& channel)
{
	std::map<std::string, Channel>& channelList = Server::getChannelList();
	for (int i = 0; i < static_cast<int>(channel.size()); i++)
		channel[i] = tolower(channel[i]);
	std::map<std::string, Channel>::iterator iter = channelList.begin();
	for (; iter != channelList.end(); iter++)
	{
		std::string name = iter->second.getName();
		for (int i = 0; i < static_cast<int>(name.size()); i++)
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
		opList.push_back(&client);
	else
		joinList.push_back(&client);
	std::vector<std::string>::iterator iter = find(inviteList.begin(), inviteList.end(), client.getNick());
	if (iter != inviteList.end())
		inviteList.erase(iter);
	
	client.addJoinedChannels(this);
}

/*  channel 나가기 
	but 클라이언트가 갖고 있는 joinList는 따로 삭제해야댐
	그리고 빈방이면 채널 삭제도 따로 해야댐
*/ 
void Channel::removeNickInChannel(Client& client)
{
	std::vector<Client*>::iterator iter = find(joinList.begin(), joinList.end(), &client);
	if (iter != joinList.end())
	{
		joinList.erase(iter);
	}

	iter = find(opList.begin(), opList.end(), &client);
	if (iter != opList.end())
	{
		opList.erase(iter);
	}
}

// channel nick 변경
void Channel::changeNickInChannel(Client& client, std::string& newNick, std::set<int>& set)
{
	// joinList에 있는 nick 변경
	std::vector<Client*>::iterator iter = find(joinList.begin(), joinList.end(), &client);
	if (iter != joinList.end())
		(*iter)->setNick(newNick);
	
	// opList에 있는 nick 변경
	iter = find(opList.begin(), opList.end(), &client);
	if (iter != opList.end())
		(*iter)->setNick(newNick);

	// joinList와 opList에 있는 클라이언트들의 fd 수집
	fillSetWithFd(set);

	// inviteList에 있는 nick 변경
	std::vector<std::string>::iterator inviteIter = find(inviteList.begin(), inviteList.end(), client.getNick());
	if (inviteIter != inviteList.end())
		*inviteIter = newNick;
}

// 채널에 있는 클라이언트들의 fd 수집
void Channel::fillSetWithFd(std::set<int>& set)
{
	std::vector<Client*>::iterator iter;

	for (iter = joinList.begin(); iter != joinList.end(); iter++)
		set.insert((*iter)->getFd());
	for (iter = opList.begin(); iter != opList.end(); iter++)
		set.insert((*iter)->getFd());
}


// 채널에 메시지 전송
void Channel::sendToClients(std::string msg)
{
	std::vector<Client*>::iterator iter = opList.begin();
	for (; iter != opList.end(); iter++)
	{
		std::cout << "member : " << (*iter)->getNick() << "\n";
		(*iter)->sendMsg(msg);
	}
	iter = joinList.begin();
	for (; iter != joinList.end(); iter++)
		(*iter)->sendMsg(msg);
}

// 채널에 메시지 전송
void Channel::sendToClients(std::string msg, Client& client)
{
	std::vector<Client*>::iterator iter = opList.begin();
	for (; iter != opList.end(); iter++)
	{
		if ((*iter)->getNick() != client.getNick())
		{
			std::cout << "member : " << (*iter)->getNick() << "\n";
			(*iter)->sendMsg(msg);
		}
	}
	iter = joinList.begin();
	for (; iter != joinList.end(); iter++)
	{
		if ((*iter)->getNick() != client.getNick())
		{
			std::cout << "member : " << (*iter)->getNick() << "\n";
			(*iter)->sendMsg(msg);
		}
	}
}

// 채널에 클라이언트 있나 확인
bool Channel::doesClientExist(std::string& nick)
{
	std::vector<Client*>::iterator iter = opList.begin();
	for (; iter != opList.end(); iter++)
	{
		if ((*iter)->getNick() == nick)
			return (true);
	}
	iter = joinList.begin();
	for (; iter != joinList.end(); iter++)
	{
		if ((*iter)->getNick() == nick)
			return (true);
	}
	return (false);
}

// 채널 추가
Channel& Channel::addChannel(std::string& channel)
{
	std::map<std::string, Channel>& channelList = Server::getChannelList();
	channelList.insert(std::make_pair(channel, Channel(channel)));
	return (findChannel(channel));
}


// 채널 삭제 (채널리스트에서 삭제)
void Channel::removeChannel(std::string channel)
{
	std::map<std::string, Channel>& channelList = Server::getChannelList();

	for (int i = 0; i < static_cast<int>(channel.size()); i++)
		channel[i] = tolower(channel[i]);
	std::map<std::string, Channel>::iterator iter = channelList.begin();
	for (; iter != channelList.end(); iter++)
	{
		std::string name = iter->second.getName();
		for (int i = 0; i < static_cast<int>(name.size()); i++)
			name[i] = tolower(name[i]);
		if (name == channel)
		{
			channelList.erase(iter);
			break ;
		}
	}
}

// 채널 리스트에서 채널 찾아주는 함수
Channel& Channel::findChannel(std::string& channel)
{
	std::map<std::string, Channel>& channelList = Server::getChannelList();

	for (int i = 0; i < static_cast<int>(channel.size()); i++)
		channel[i] = tolower(channel[i]);
	std::map<std::string, Channel>::iterator iter = channelList.begin();
	for (; iter != channelList.end(); iter++)
	{
		std::string name = iter->second.getName();
		for (int i = 0; i < static_cast<int>(name.size()); i++)
			name[i] = tolower(name[i]);
		if (name == channel)
			break;
	}
	return (iter->second);
}

// mode 함수
bool Channel::isKeyMode()
{
	if (mode.find("k") != std::string::npos)
		return (true);
	return (false);
}

bool Channel::isInviteMode()
{
	if (mode.find("i") != std::string::npos)
		return (true);
	return (false);

}

bool Channel::isLimitMode()
{
	if (mode.find("l") != std::string::npos)
		return (true);
	return (false);
}

bool Channel::doesTopicExist()
{
	return (topicFlag);
}

bool Channel::isInvitedClient(std::string nick)
{
	if (find(inviteList.begin(), inviteList.end(), nick) == inviteList.end())
		return (false);
	return (true);
}


void Channel::removeClientFromInvitedList(std::string nick)
{
	inviteList.erase(find(inviteList.begin(), inviteList.end(), nick));
}


// getter
std::string Channel::getName()
{
	return (channelName);
}

std::string Channel::getKey()
{
	return (key);
}

int Channel::getLimit()
{
	return (limit);
}

int Channel::getSize()
{
	return (joinList.size() + opList.size());
}

std::string Channel::getTopic()
{
	return (topic);
}

std::string Channel::getTopicWriter()
{
	return (topicWriter);
}

int	Channel::getTopicTime()
{
	return (timeTopicWasWritten);
}

std::vector<Client*>& Channel::getJoinList()
{
	return (joinList);
}

std::vector<Client*>& Channel::getOpList()
{
	return (opList);
}

std::vector<std::string>& Channel::getInviteList()
{
	return (inviteList);
}


// setter
void Channel::setTopic(Client& client, std::string topic)
{
	topicFlag = true;
	this->topic = topic;
	topicWriter = client.getNick();
	timeTopicWasWritten = time(0);
}
