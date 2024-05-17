#include "Executor.hpp"
#include "Server.hpp"

void Executor::PONG(Client& client, std::vector<std::string>& cmds)
{
	if (cmds.size() < 3)
		client.sendMsg(ServerMsg::NOORIGIN(client.getNick()));
	else if (cmds[1] != Server::IP)
		client.sendMsg(ServerMsg::NOSUCHSERVER(client.getNick(), cmds[1]));
	else if (cmds[2] != (":" + Server::IP))
		client.sendMsg(ServerMsg::NOSUCHSERVER(client.getNick(), cmds[2].substr(1)));
	else
	{
		client.sendMsg(ServerMsg::WELCOME(client.getNick(), client.getHostName(), client.getServerName()));
		client.sendMsg(ServerMsg::NOMOTD(client.getNick()));
		return ;
	}
	Server::clientList.erase(client.getFd());
}