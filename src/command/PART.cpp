#include "../../include/core/Executor.hpp"
#include "../../include/core/Server.hpp"
void Executor::parsePART(std::vector<std::string>& cmds, std::string& msg)
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

void Executor::PART(Client& client, std::vector<std::string>& cmds)
{
	// 인자 부족
	if (cmds.size() < 2)
	{
		client.addToSendBuf(ServerMsg::NEEDMOREPARAMS(client.getNick(), cmds[0]));
		return ;
	}
	
	// PART 명령어 파싱 2탄
	// 1번째 인자의 ','를 ' '로 변경
	for (int i = 0; i < static_cast<int>(cmds[1].size()); i++)
	{
		if (cmds[1][i] == ',')
			cmds[1][i] = ' ';
	}
	// 첫 번째 인자를 ' '기준으로 나눔
	std::vector<std::string>  roomVector;
	std::stringstream ss(cmds[1]);
	std::string str;
	while (ss >> str)
		roomVector.push_back(str);

	for (int i = 0; i < static_cast<int>(roomVector.size()); i++)
	{
		std::string room = roomVector[i];

		// 채널이 존재하지 않는경우
		if (!Channel::isChannelInUse(room))
		{
			client.addToSendBuf(ServerMsg::NOSUCHCHANNEL(client.getNick(), room));
			continue ;
		}

		// 내가 해당 채널에 없는 경우
		Channel& channel = Channel::getChannel(room);
		if (!channel.doesClientExist(client.getNick()))
		{
			client.addToSendBuf(ServerMsg::NOTONCHANNEL(client.getNick(), channel.getName()));
			continue ;
		}

		if (cmds.size() == 2) // 메시지가 없는 경우
		{
			channel.addMsgToClientsSendBuf(ServerMsg::PART(client.getNick(), client.getHostName(), client.getServerName(), room, ""));
		}
		else // 메시지가 있는 경우
		{
			channel.addMsgToClientsSendBuf(ServerMsg::PART(client.getNick(), client.getHostName(), client.getServerName(), room, cmds[2]));
		}
		channel.removeNickInChannel(client);
		client.removeJoinedChannels(&channel);
		if (channel.getSize() == 0)
				Channel::removeChannel(channel.getName());
	}
}
