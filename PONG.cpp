#include "Executor.hpp"
#include "Server.hpp"

void Executor::parsePONG(std::vector<std::string>& cmds, std::string& msg)
{
	int size = msg.size();
	int argNum = 0;
	int i = 0;
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

	while(i < size && msg[i] != ':')
		i++;
	if (++i < size)
		cmds.push_back(msg.substr(i));
}

void Executor::PONG(Client& client, std::vector<std::string>& cmds)
{
	if (cmds.size() < 3)
		client.sendMsg(ServerMsg::NOORIGIN(client.getNick()));
	else if (cmds[1] != Server::getIP())
		client.sendMsg(ServerMsg::NOSUCHSERVER(client.getNick(), cmds[1]));
	else if (cmds[2] != (Server::getIP()))
		client.sendMsg(ServerMsg::NOSUCHSERVER(client.getNick(), cmds[2]));
	else
	{
		client.sendMsg(ServerMsg::WELCOME(client.getNick(), client.getHostName(), client.getServerName()));
		client.sendMsg(ServerMsg::NOMOTD(client.getNick()));
		return ;
	}
	std::map<int, Client>& clientList = Server::getClientList();
	clientList.erase(client.getFd());
}