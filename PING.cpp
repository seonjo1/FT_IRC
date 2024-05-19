#include "Executor.hpp"
#include "Server.hpp"

void Executor::parsePING(std::vector<std::string>& cmds, std::string& msg)
{
	std::stringstream ss(msg);
	std::string cmd;
	while (ss >> cmd)
		cmds.push_back(cmd);
}	

void Executor::PING(Client& client, std::vector<std::string>& cmds)
{
	if (cmds.size() == 1)
	{
		client.sendMsg(ServerMsg::NOORIGIN(client.getNick()));
	}
	else if (cmds.size() == 2)
	{
		client.sendMsg(ServerMsg::PING(cmds[1], ""));
	}
	else if (cmds.size() == 2)
	{
		client.sendMsg(ServerMsg::PING(cmds[1], cmds[2]));
	}
}

