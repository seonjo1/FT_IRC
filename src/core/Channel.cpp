#include "../../include/core/Server.hpp"
#include "../../include/core/Channel.hpp"

Channel::Channel(std::string& channelName)
	: channelName(channelName), timeChannelWasCreated(time(0)), topicFlag(false) {};

Channel::~Channel() {};

// 채널에 참가
void Channel::joinChannel(Client& client, std::string& channelName, std::string param)
{
	if (isChannelInUse(channelName)) // 기존 채널이 있는 경우
	{
		// channel 찾기
		Channel& channel = getChannel(channelName);
		
		// key 모드인데 key가 틀린경우
		if (channel.isKeyMode() && channel.getKey() != param)
		{
			client.addToSendBuf(ServerMsg::BADCHANNELKEY(client.getNick(), channel.getName()));
			return ;
		}

		// invite 여부 확인
		if (channel.isInviteMode() && !channel.isInvitedClient(client.getNick()))
		{
			client.addToSendBuf(ServerMsg::INVITEONLYCHAN(client.getNick(), channel.getName()));
			return ;
		}

		// 풀방 확인
		if (channel.isLimitMode() && irc_atoi(channel.getLimit()) <= channel.getSize())
		{
			client.addToSendBuf(ServerMsg::CHANNELISFULL(client.getNick(), channel.getName()));
			return ;
		}

		// 채널에 참여
		channel.addNickInChannel(client);

		// 채널에 모든이들에게 join 메시지전송
		channel.addMsgToClientsSendBuf(ServerMsg::JOIN(client.getNick(), client.getHostName(),
											client.getServerName(), channel.getName()));
		
		// 토픽확인하고 참가자에게 토픽 메시지 전송
		if (channel.doesTopicExist())
		{
			client.addToSendBuf(ServerMsg::TOPIC(client.getNick(), channel.getName(), channel.getTopic()));
			client.addToSendBuf(ServerMsg::TOPICINFO(client.getNick(), channel.getName(),
												channel.getTopicWriter(), channel.getTopicTime()));
		}

		// 참가자 리스트 전송
		client.addToSendBuf(ServerMsg::NAMREPLY(client.getNick(), channel));
		client.addToSendBuf(ServerMsg::ENDOFNAMES(client.getNick(), channel.getName()));
	}
	else // 채널이 없어서 만들어야하는 경우
	{
		Channel& channel = addChannel(channelName);

		// 채널에 참여
		channel.addNickInChannel(client);

		// join 메시지전송
		client.addToSendBuf(ServerMsg::JOIN(client.getNick(), client.getHostName(),
									 client.getServerName(), channel.getName()));

		// 참가자 리스트 전송
		client.addToSendBuf(ServerMsg::NAMREPLY(client.getNick(), channel));
		client.addToSendBuf(ServerMsg::ENDOFNAMES(client.getNick(), channel.getName()));
	}
}


 // channel 이름 유효성 검사
bool Channel::isInvalidChannelName(std::string channel)
{
	// 규칙 1. 맨 앞의 char는 '#' or '&'
	if (channel[0] != '#' && channel[0] != '&')
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
bool Channel::isChannelInUse(std::string channel)
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

	removeFromInvitedList(client.getNick());
	
	client.addJoinedChannels(this);
}

/*  channel 나가기 
	but 클라이언트가 갖고 있는 joinChannels는 따로 삭제해야댐
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
void Channel::changeNickInChannel(Client& client, std::string& newNick, std::set<Client*>& set)
{
	// joinList와 opList에 있는 클라이언트들의 fd 수집
	fillSet(set);

	// inviteList에 있는 nick 변경
	std::vector<std::string>::iterator inviteIter = find(inviteList.begin(), inviteList.end(), client.getNick());
	if (inviteIter != inviteList.end())
		*inviteIter = newNick;
}

// 채널에 있는 클라이언트들의 fd 수집
void Channel::fillSet(std::set<Client*>& set)
{
	std::vector<Client*>::iterator iter;

	for (iter = joinList.begin(); iter != joinList.end(); iter++)
		set.insert(*iter);
	for (iter = opList.begin(); iter != opList.end(); iter++)
		set.insert(*iter);
}


// 채널에 메시지 전송
void Channel::addMsgToClientsSendBuf(std::string msg)
{
	std::vector<Client*>::iterator iter = opList.begin();
	for (; iter != opList.end(); iter++)
		(*iter)->addToSendBuf(msg);
	iter = joinList.begin();
	for (; iter != joinList.end(); iter++)
		(*iter)->addToSendBuf(msg);
}

// 채널에 메시지 전송
void Channel::addMsgToClientsSendBuf(std::string msg, Client& client)
{
	std::vector<Client*>::iterator iter = opList.begin();
	for (; iter != opList.end(); iter++)
	{
		if ((*iter)->getNick() != client.getNick())
			(*iter)->addToSendBuf(msg);
	}
	iter = joinList.begin();
	for (; iter != joinList.end(); iter++)
	{
		if ((*iter)->getNick() != client.getNick())
			(*iter)->addToSendBuf(msg);
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
	return (getChannel(channel));
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
Channel& Channel::getChannel(std::string& channel)
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

bool Channel::isInvalidKey(std::string key)
{
	int size = key.size();
	for (int i = 0; i < size; i++)
	{
		if (!isprint(key[i]))
			return (true);
	}
	return (false);
}

bool Channel::isInvalidLimit(std::string limit)
{
	// 숫자가 0 ~ 1000일 때만 허용
	int num = irc_atoi(limit);
	if (num == -1)
		return (true);
	return (false);
}

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

bool Channel::isTopicMode()
{
	if (mode.find("t") != std::string::npos)
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

bool Channel::isOperator(std::string nick)
{
	std::vector<Client*>::iterator iter = opList.begin();
	for (; iter != opList.end(); iter++)
	{
		if ((*iter)->getNick() == nick)
			return (true);
	}
	return (false);
}

// Invite List 에 클라이언트 추가
void Channel::addToInviteList(std::string nick)
{
	if (find(inviteList.begin(), inviteList.end(), nick) == inviteList.end())
		inviteList.push_back(nick);
}

// Invite List 에서 클라이언트 삭제
void Channel::removeFromInvitedList(std::string nick)
{	
	std::vector<std::string>::iterator iter = find(inviteList.begin(), inviteList.end(), nick);
	if (iter != inviteList.end())
		inviteList.erase(iter);
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

int Channel::getSize()
{
	return (joinList.size() + opList.size());
}

int	Channel::getTopicTime()
{
	return (timeTopicWasWritten);
}

int Channel::getCreatedTime()
{
	return (timeChannelWasCreated);
}


bool Channel::getTopicFlag()
{
	return (topicFlag);
}

Bot& Channel::getBot()
{
	return (bot);
}

std::string Channel::getLimit()
{
	return (limit);
}

std::string Channel::getTopic()
{
	return (topic);
}

std::string Channel::getTopicWriter()
{
	return (topicWriter);
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

std::string Channel::getModeInfo(std::string nick)
{
	// mode + 요소 modeInfo에 저장
	std::string modeInfo("+" + mode);

	// 만약 parma을 갖는 요소가 있으면 따로 저장
	std::vector<std::string> v;
	int size = mode.size();
	for (int i = 0; i < size; i++)
	{
		if (mode[i] == 'k')
		{
			if (doesClientExist(nick))
				v.push_back(key);
			else
				v.push_back("<key>");
		}
		else if (mode[i] == 'l')
			v.push_back(limit);
	}

	// modeInfo에 param들 순서대로 저장 (마지막 요소는 ':'를 붙여서 저장)
	size = v.size();
	for (int i = 0; i < size; i++)
	{
		if (i == size - 1)
			modeInfo += " :" + v[i];
		else
			modeInfo += " " + v[i];
	}

	if (modeInfo.size() == 1)
		return ("");
	// modeInfo를 반환
	return (modeInfo);
}

// setter
void Channel::setTopic(std::string nick, std::string topic)
{
	topicFlag = true;
	this->topic = topic;
	topicWriter = nick;
	timeTopicWasWritten = time(0);
}

void Channel::setInviteMode(bool sign)
{
	if (sign) // +i
		mode += "i";
	else //-i
		mode.erase(mode.find("i"), 1);
}

void Channel::setTopicMode(bool sign)
{
	if (sign) // +t
		mode += "t";
	else //-t
		mode.erase(mode.find("t"), 1);
}

void Channel::setKeyMode(bool sign, std::string key)
{
	if (sign) // +k
	{
		mode += "k";
		this->key = key;
	}
	else //-k
		mode.erase(mode.find("k"), 1);
}

void Channel::setLimitMode(bool sign, std::string limit)
{
	if (sign) // +l
	{
		mode += "l";
		this->limit = limit;
	}
	else //-l
		mode.erase(mode.find("l"), 1);
}

void Channel::setOPMode(bool sign, std::string nick)
{

	if (sign) // +o
	{
		std::vector<Client*>::iterator iter;
		for (iter = joinList.begin(); iter != joinList.end(); iter++)
		{
			if ((*iter)->getNick() == nick)
			{
				opList.push_back(*iter);
				joinList.erase(iter);
				break ;
			}
		}
	}
	else //-o
	{
		std::vector<Client*>::iterator iter;
		for (iter = opList.begin(); iter != opList.end(); iter++)
		{
			if ((*iter)->getNick() == nick)
			{
				joinList.push_back(*iter);
				opList.erase(iter);
				break ;
			}
		}
	}
}
