#include "Executor.hpp"
#include "Server.hpp"

Executor::Executor(char *password)
	: password(password) {};

Executor::~Executor() {};

void Executor::execute(Client& client, std::string msg)
{
	std::cout << "cmd : " << msg << "\n";
	std::vector<std::string> cmds = parseMsg(msg);

	for (std::vector<std::string>::iterator iter = cmds.begin(); iter != cmds.end(); iter++)
		std::cout << *iter << "\n";

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
			return ;
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
	}
}

std::vector<std::string> Executor::parseMsg(std::string &msg)
{
	std::vector<std::string> cmds;
	
	int i = 0;
	int size = msg.size();
	std::string cmd;
	while (i < size && msg[i] != ' ')
		cmd += msg[i++];

	if (cmd == "PASS")
		parsePASS(cmds, msg);
	else if (cmd == "NICK")
		parseNICK(cmds, msg);
	else if (cmd == "USER")
		parseUSER(cmds, msg);
	else if (cmd == "PONG")
		parsePONG(cmds, msg);
	else
		cmds.push_back(msg);
	return (cmds);
}
