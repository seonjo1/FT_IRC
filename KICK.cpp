#include "Executor.hpp"
#include "Server.hpp"

void Executor::parseKICK(std::vector<std::string>& cmds, std::string& msg)
{
	int size = msg.size();
	int argNum = 0;
	int i = 0;

	// 공백 기준으로 앞에 인자 3개 구분
	while (1)
	{
		if (argNum == 3 || i == size) break;
		while (i < size && msg[i] == ' ')
			i++;
		std::string cmd;
		while (i < size && msg[i] != ' ')
			cmd += msg[i++];
		if (cmd.size() > 0)
		{
			cmds.push_back(cmd);
			argNum++;
		}
	}

	// 4번째 인자는 ':'으로 시작하면 인정
	while(i < size && msg[i] != ':')
	{
		// 3번째 인자가 ':'으로 시작안하는 애면 오류
		if (msg[i] != ' ')
		{
			cmds.push_back("error");
			cmds.push_back("error");
			return ;
		}
		i++;
	}

	// 4번째 인자 추가
	if (++i < size)
		cmds.push_back(msg.substr(i));
}

void Executor::KICK(Client& client, std::vector<std::string>& cmds)
{
	// 인자 개수 오류
	if (cmds.size() != 3 && cmds.size() != 4)
	{
		client.sendMsg(ServerMsg::NEEDMOREPARAMS(client.getNick(), cmds[0]));
		return ;
	}

	// 없는 채널
	if (!Channel::isChannelInUse(cmds[1]))
	{
		client.sendMsg(ServerMsg::NOSUCHCHANNEL(client.getNick(), cmds[1]));
		return ;
	}

	// 없는 닉
	if (!Client::isNicknameInUse(cmds[2]))
	{
		client.sendMsg(ServerMsg::NOSUCHNICK(client.getNick(), cmds[2]));
		return ;
	}

	// 내가 채널에 없을때
	if (!client.isClientMemberOfChannel(cmds[1]))
	{ 
		client.sendMsg(ServerMsg::NOTONCHANNEL(client.getNick(), cmds[1]));
		return ;
	}

	// kick 하려는 사람이 채널에 없을 때
	Channel& channel = Channel::getChannel(cmds[1]);
	if (!channel.doesClientExist(cmds[2]))
	{
		client.sendMsg(ServerMsg::USERNOTINCHANNEL(client.getNick(), cmds[2], cmds[1]));
		return ;
	}

	// client가 operator 권한이 없을 때
	if (!channel.isOperator(client.getNick()))
	{
		client.sendMsg(ServerMsg::CHANOPRIVSNEEDED(client.getNick(), cmds[1]));
		return ;
	}

	// kick! 메시지 전송
	if (cmds.size() == 3)
	{
		channel.sendToClients(ServerMsg::KICK(client.getNick(), client.getHostName(), client.getServerName(),
												channel.getName(), cmds[2], ""));
	}
	else
	{
		channel.sendToClients(ServerMsg::KICK(client.getNick(), client.getHostName(), client.getServerName(),
												channel.getName(), cmds[2], cmds[3]));
	}

	// 채널 리스트에서 클라이언트 제거
	Client& beKickedClient = Client::getClient(cmds[2]);
	channel.removeNickInChannel(beKickedClient);

	// 클라이언트 채널 리스트에서 채널 제거
	beKickedClient.removeJoinedChannels(&channel);
}
