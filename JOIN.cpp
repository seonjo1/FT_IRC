#include "Executor.hpp"
#include "Server.hpp"

void Executor::parseJOIN(std::vector<std::string>& cmds, std::string& msg)
{
	std::stringstream ss(msg);
	std::string cmd;
	while (ss >> cmd)
		cmds.push_back(cmd);
}

void Executor::JOIN(Client& client, std::vector<std::string>& cmds)
{
	// 매개변수가 부족한 경우
	if (cmds.size() == 1)
	{
		client.sendMsg(ServerMsg::NEEDMOREPARAMS(client.getNick(), cmds[0]));
		return ;
	}
	// 잘못된 채널 이름인 경우
	if (Channel::isInvalidChannelName(cmds[1]))
	{
		client.sendMsg(ServerMsg::INVALIDCHANNEL(client.getNick(), cmds[1]));
		return ;
	}
	// 이미 참여한 채널인 경우
	if (client.isClientMemberOfChannel(cmds[1]))
	{
		return ;
	}

	// 채널에 참가
	if (cmds.size() == 2)
		Channel::joinChannel(client, cmds[1], "");
	else
		Channel::joinChannel(client, cmds[1], cmds[2]);
}