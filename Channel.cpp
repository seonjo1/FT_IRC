#include "Channel.hpp"

Channel::Channel()
{
}

Channel::Channel(const Channel& copy)
{
}

Channel::~Channel()
{
}

Channel& Channel::operator=(const Channel& copy)
{
}

void Channel::removeNick(std::string& nick)
{
	std::vector<std::string>::iterator iter = find(clientList.begin(), clientList.end(), nick);
	if (iter != clientList.end()) clientList.erase(iter);
}

