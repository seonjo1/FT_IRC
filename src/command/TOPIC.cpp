#include "../../include/core/Executor.hpp"
#include "../../include/core/Server.hpp"

void Executor::parseTOPIC(std::vector<std::string>& cmds, std::string& msg)
{
	int size = msg.size();
	int argNum = 0;
	int i = 0;

	// 공백 기준으로 앞에 인자 2개 구분
	while (1)
	{
		if (argNum == 2 || i == size) break;
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

	// 3번째 인자는 ':'으로 시작하면 인정
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

	// 3번째 인자 추가
	if (++i < size)
		cmds.push_back(msg.substr(i));
}

void Executor::TOPIC(Client& client, std::vector<std::string>& cmds)
{
	// 인자 부족
	if (cmds.size() != 2 && cmds.size() != 3)
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
	
	Channel& channel = Channel::getChannel(cmds[1]);
	// 인자가 2개인 경우
	if (cmds.size() == 2)
	{
		if (channel.getTopicFlag()) // topic이 있는 경우
		{
			client.sendMsg(ServerMsg::TOPIC(client.getNick(), cmds[1], channel.getTopic()));
			client.sendMsg(ServerMsg::TOPICINFO(client.getNick(), cmds[1], channel.getTopicWriter(), channel.getTopicTime()));
		}
		else	// topic이 없는 경우
		{
			client.sendMsg(ServerMsg::NOTOPIC(client.getNick(), cmds[1]));
		}
		return ;
	}

	// 내가 그 채널에 없을 때
	if (!channel.doesClientExist(client.getNick()))
	{
		client.sendMsg(ServerMsg::NOTONCHANNEL(client.getNick(), cmds[1]));
		return ;
	}

	// 오퍼레이터가 아닐때 (권한 없을 때)
	if (!channel.isOperator(client.getNick()) && channel.isTopicMode())
	{
		client.sendMsg(ServerMsg::CHANOPRIVSNEEDEDTOPICVERSION(client.getNick(), cmds[1]));
		return ;
	}

	// TOPIC 변경
	channel.setTopic(client.getNick(), cmds[2]);

	// TOPIC 변경사항 전달
	channel.sendToClients(ServerMsg::TOPICCHANGE(client.getNick(), client.getHostName(), client.getServerName(),
												cmds[1], cmds[2]));
}

