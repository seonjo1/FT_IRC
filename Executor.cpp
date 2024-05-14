#include "Executor.hpp"

Executor::Executor(char *password)
	: password(password) {};

Executor::~Executor() {};

void Executor::execute(Client& client, std::string msg)
{
	std::vector<std::string> cmds = splitMsg(msg);

	if (!client.getPassFlag() || !client.getNickFlag() || !client.getUserFlag())
	{
		if (cmds[0] == "PASS")
			PASS(client, cmds);
		else if (cmds[0] == "NICK")
			NICK(client, cmds);
		else if (cmds[0] == "USER")
			USER(client, cmds);
		
		if (client.getPassFlag() && client.getNickFlag() && client.getUserFlag())

	}
	else
	{
		if (cmds[0] == "PASS")
			PASS(client, cmds);
		else if (cmds[0] == "NICK")
			NICK(client, cmds);
		else if (cmds[0] == "USER")
			USER(client, cmds);
	}
}

std::vector<std::string> Executor::splitMsg(std::string &msg)
{
	std::vector<std::string> cmds;
	std::stringstream ss(msg);

	std::string cmd;
	while (ss >> cmd)
		cmds.push_back(cmd);
	
	return (cmds);
}
