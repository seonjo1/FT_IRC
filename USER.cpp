#include "Executor.hpp"
#include "Server.hpp"

void Executor::parseUSER(std::vector<std::string>& cmds, std::string& msg)
{
	int size = msg.size();
	int argNum = 0;
	int i = 0;
	while (1)
	{
		if (argNum == 4 || i == size) break;
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

	while (i < size && msg[i] != ':')
		i++;
	if (++i != size)
		cmds.push_back(msg.substr(i));
}

void Executor::USER(Client& client, std::vector<std::string>& cmds)
{
	// PASS 등록 안된 클라이언트
	if (!client.getPassFlag())
	{
		client.sendMsg(ServerMsg::NOTREGISTERD(client.getNick()));
		// 연결 종료
		std::map<int, Client>& clientList = Server::getClientList();
		clientList.erase(client.getFd());
	}

	// 인자 개수가 다른 경우
	if (cmds.size() < 5)
	{
		client.sendMsg(ServerMsg::NEEDMOREPARAMS(client.getNick(), cmds[0]));
		return ;
	}

	// 이미 user 등록이 완료된 client
	if (client.getUserFlag())
	{
		client.sendMsg(ServerMsg::ALREADYREGISTER(client.getNick()));
		return ;
	}

	client.setData(cmds[1], cmds[2], cmds[3], cmds[4]);
	client.setUserFlag(true);
}