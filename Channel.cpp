#include "Channel.hpp"

Channel::Channel(std::string& channelName)
    : channelName(channelName)
{
    timeChannelWasCreated = time();
}

Channel::~Channel();

 // channel 이름 유효성 검사
bool Channel::isInvalidChannelName(std::string& channel)
{

}

// channel이 존재하는지 확인
bool Channel::isChannelInUse(std::string& channel)
{
    std::map<std::string, Channel>::iterator iter = Server::channelList.begin();
    for (; iter != Server::channelList.end(); iter++)
    {
        if (iter->second.getName() == channel)
            return (true);
    }
    return (false);
}

// channel에 참가
void Channel::addNickInChannel(std::string& nick)
{

}

// channel 나가기
void Channel::removeNickInChannel(std::string& nick)
{

}

// channel nick 변경
void Channel::changeNickInChannel(std::string& oldNick, std::string& newNick)
{

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
        if (iter->nickname == nick)
            return (true);
    }
    iter = joinList.begin();
    for (; iter != joinList.begin(); iter++)
    {
        if (iter->nickname == nick)
            return (true);
    }
    return (false);
}

// 채널 삭제 (채널리스트에서 삭제)
void Channel::removeChannel(std::string& channel)
{
    std::map<std::string, Channel>::iterator iter = Server::channelList.begin();
    for (; iter != Server::channelList.end(); iter++)
    {
        if (iter->second.getName() == channel)
            Server::channelList.erase(iter);
    }
}

// 채널 리스트에서 채널 찾아주는 함수
Channel& Channel::findChannel(std::string& channel)
{
    std::map<std::string, Channel>::iterator iter = Server::channelList.begin();
    for (; iter != Server::channelList.end(); iter++)
    {
        if (iter->second.getName() == channel)
            return (iter->second);
    }
}

// getter
std::string Channel::getName()
{
    return (channelName);
}