#include "../../include/core/Executor.hpp"
#include "../../include/core/Server.hpp"

void Executor::parsePING(std::vector<std::string>& cmds, std::string& msg)
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

void Executor::PING(Client& client, std::vector<std::string>& cmds)
{
	if (cmds.size() == 1)
	{
		client.addToSendBuf(ServerMsg::NOORIGIN(client.getNick()));
	}
	else if (cmds.size() == 2)
	{
		client.addToSendBuf(ServerMsg::PING(cmds[1], ""));
	}
	else
	{
		client.addToSendBuf(ServerMsg::PING(cmds[1], cmds[2]));
	}
}

