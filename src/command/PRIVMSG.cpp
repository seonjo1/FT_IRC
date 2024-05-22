#include "../../include/core/Executor.hpp"
#include "../../include/core/Server.hpp"

void Executor::parsePRIVMSG(std::vector<std::string>& cmds, std::string& msg)
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
			return ;
		i++;
	}

	// 3번째 인자 추가
	if (++i < size)
		cmds.push_back(msg.substr(i));
}

void Executor::PRIVMSG(Client& client, std::vector<std::string>& cmds)
{
	// 리시버가 없는 경우
	if (cmds.size() == 1)
	{
		client.sendMsg(ServerMsg::NORECIPIENT(client.getNick(), cmds[0]));
		return ;
	}

	// PRIVMSG 명령어 파싱 2탄!
	// 1번째 인자의 ','를 ' '로 변경
	for (int i = 0; i < static_cast<int>(cmds[1].size()); i++)
	{
		if (cmds[1][i] == ',')
			cmds[1][i] = ' ';
	}
	// 첫 번째 인자를 ' '기준으로 나눔
	std::vector<std::string>  receiverVector;
	std::stringstream ss(cmds[1]);
	std::string str;
	while (ss >> str)
		receiverVector.push_back(str);

	for (int i = 0; i < static_cast<int>(receiverVector.size()); i++)
	{
		std::string receiver = receiverVector[i];

		// 메시지가 없는 경우
		if (cmds.size() == 2)
		{
			client.sendMsg(ServerMsg::NOTEXTTOSEND(client.getNick()));
			continue ;
		}
	
		if (receiver[0] == '#' || receiver[0] == '&') // receiver가 채널인 경우
		{
			// 채널이 없는 경우
			if (!Channel::isChannelInUse(receiver))
			{
				client.sendMsg(ServerMsg::NOSUCHCHANNEL(client.getNick(), receiver));
				continue ;
			}

			// 해당 채널에 클라이언트가 없는 경우
			Channel& channel = Channel::getChannel(receiver);
			if (!channel.doesClientExist(client.getNick()))
			{
				client.sendMsg(ServerMsg::NOTONCHANNEL(client.getNick(), channel.getName()));
				continue ;
			}

			// 메시지 전달
			channel.sendToClients(ServerMsg::PRIVMSG(client.getNick(), client.getHostName(),
													client.getServerName(), receiver, cmds[2]),
									client);
		}
		else  // receiver가 닉네임인 경우
		{	
			// nick이 없는 경우
			if (!Client::isNicknameInUse(receiver))
			{
				client.sendMsg(ServerMsg::NOSUCHNICK(client.getNick(), receiver));
				continue ;
			}
			
			// 메시지 전달
			Client& toClient = Client::getClient(receiver);
			toClient.sendMsg(ServerMsg::PRIVMSG(client.getNick(), client.getHostName(),
											client.getServerName(), receiver, cmds[2]));
		}
	}
}
