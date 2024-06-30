#include "../../include/core/Executor_bonus.hpp"
#include "../../include/core/Server_bonus.hpp"

// TOPIC 파싱
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

	// 공백 다 pass
	while(i < size && msg[i] == ' ')
		i++;

	// ':'가 있으면 index 1 증가
	if (i < size && msg[i] == ':')
		i++;
	
	// 뒤에 내용이 있는경우 한번에 담기
	if (i < size)
		cmds.push_back(msg.substr(i));
}

// TOPIC 실행
void Executor::TOPIC(Client& client, std::vector<std::string>& cmds)
{
	// 인자 부족
	if (cmds.size() < 2)
	{
		client.addToSendBuf(ServerMsg::NEEDMOREPARAMS(client.getNick(), cmds[0]));
		return ;
	}

	// 없는 채널
	if (!Channel::isChannelInUse(cmds[1]))
	{
		client.addToSendBuf(ServerMsg::NOSUCHCHANNEL(client.getNick(), cmds[1]));
		return ;
	}
	
	Channel& channel = Channel::getChannel(cmds[1]);
	// 인자가 2개인 경우
	if (cmds.size() == 2)
	{
		if (channel.getTopicFlag()) // topic이 있는 경우
		{
			client.addToSendBuf(ServerMsg::TOPIC(client.getNick(), cmds[1], channel.getTopic()));
			client.addToSendBuf(ServerMsg::TOPICINFO(client.getNick(), cmds[1], channel.getTopicWriter(), channel.getTopicTime()));
		}
		else	// topic이 없는 경우
		{
			client.addToSendBuf(ServerMsg::NOTOPIC(client.getNick(), cmds[1]));
		}
		return ;
	}

	// 내가 그 채널에 없을 때
	if (!channel.doesClientExist(client.getNick()))
	{
		client.addToSendBuf(ServerMsg::NOTONCHANNEL(client.getNick(), cmds[1]));
		return ;
	}

	// 오퍼레이터가 아닐때 (권한 없을 때)
	if (!channel.isOperator(client.getNick()) && channel.isTopicMode())
	{
		client.addToSendBuf(ServerMsg::CHANOPRIVSNEEDEDTOPICVERSION(client.getNick(), cmds[1]));
		return ;
	}

	// TOPIC 변경
	channel.setTopic(client.getNick(), cmds[2]);

	// TOPIC 변경사항 전달
	channel.addMsgToClientsSendBuf(ServerMsg::TOPICCHANGE(client.getNick(), client.getHostName(), client.getServerName(),
												cmds[1], cmds[2]));
}

