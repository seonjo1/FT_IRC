#include "../../include/core/Executor_bonus.hpp"
#include "../../include/core/Server_bonus.hpp"

// KICK 파싱
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

// KICK 실행
void Executor::KICK(Client& client, std::vector<std::string>& cmds)
{
	// 인자 개수 오류
	if (cmds.size() < 3)
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

	// 없는 닉
	if (!Client::isNicknameInUse(cmds[2]))
	{
		client.addToSendBuf(ServerMsg::NOSUCHNICK(client.getNick(), cmds[2]));
		return ;
	}

	// 내가 채널에 없을때
	if (!client.isClientMemberOfChannel(cmds[1]))
	{ 
		client.addToSendBuf(ServerMsg::NOTONCHANNEL(client.getNick(), cmds[1]));
		return ;
	}

	// kick 하려는 사람이 채널에 없을 때
	Channel& channel = Channel::getChannel(cmds[1]);
	if (!channel.doesClientExist(cmds[2]))
	{
		client.addToSendBuf(ServerMsg::USERNOTINCHANNEL(client.getNick(), cmds[2], cmds[1]));
		return ;
	}

	// client가 operator 권한이 없을 때
	if (!channel.isOperator(client.getNick()))
	{
		client.addToSendBuf(ServerMsg::CHANOPRIVSNEEDED(client.getNick(), cmds[1]));
		return ;
	}

	// kick! 메시지 전송
	if (cmds.size() == 3)
	{
		channel.addMsgToClientsSendBuf(ServerMsg::KICK(client.getNick(), client.getHostName(), client.getServerName(),
												channel.getName(), cmds[2], ""));
	}
	else
	{
		channel.addMsgToClientsSendBuf(ServerMsg::KICK(client.getNick(), client.getHostName(), client.getServerName(),
												channel.getName(), cmds[2], cmds[3]));
	}

	// 채널 리스트에서 클라이언트 제거
	Client& beKickedClient = Client::getClient(cmds[2]);
	channel.removeNickInChannel(beKickedClient);

	// 클라이언트 채널 리스트에서 채널 제거
	beKickedClient.removeJoinedChannels(&channel);

	// 만약 채널에 아무도 없으면 채널 제거
	if (channel.getSize() == 0)
		Channel::removeChannel(channel.getName());
}
