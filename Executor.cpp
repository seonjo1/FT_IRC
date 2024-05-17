#include "Executor.hpp"
#include "Server.hpp"

// #include <iostream>

Executor::Executor(char *password)
	: password(password) {};

Executor::~Executor() {};

void Executor::execute(Client& client, std::string msg)
{
	std::vector<std::string> cmds = splitMsg(msg);

	// std::cout << "COMMAND : " << cmds[0] << "\n";

	// 클라이언트 등록이 안 되있는 경우
	if (!client.isRegistered())
	{
		if (cmds[0] == "PASS") // PASS 명령어 실행
			PASS(client, cmds);
		else if (cmds[0] == "NICK") // NICK 명령어 실행
			NICK(client, cmds);
		else if (cmds[0] == "USER") // USER 명령어 실행
			USER(client, cmds);
		else // 그 외의 명령일 경우 연결 종료
		{
			client.sendMsg(ServerMsg::NOTREGISTERD(client.getNick()));
			std::map<int, Client>& clientList = Server::getClientList();
			clientList.erase(client.getFd());
		}
		// 클라이언트 등록이 완료된 경우 완료 메시지 전송
		if (client.isRegistered())
			client.sendMsg(ServerMsg::PINGMSG(Server::getIP()));
	}
	else
	{
		if (cmds[0] == "PASS")
			PASS(client, cmds);
		else if (cmds[0] == "NICK")
			NICK(client, cmds);
		else if (cmds[0] == "USER")
			USER(client, cmds);
		else if (cmds[0] == "PONG")
			PONG(client, cmds);
		else
			std::cout << "cmd : " << cmds[0] << "\n";
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
