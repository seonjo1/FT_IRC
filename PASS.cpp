#include "Executor.hpp"
#include "Server.hpp"

void Executor::parsePASS(std::vector<std::string>& cmds, std::string& msg)
{
	std::stringstream ss(msg);
	std::string cmd;
	while (ss >> cmd)
		cmds.push_back(cmd);
}


void Executor::PASS(Client& client, std::vector<std::string>& cmds)
{
	// 이미 PASS 인증이 끝난 유저
	if (client.getPassFlag())
	{
		client.sendMsg(ServerMsg::ALREADYREGISTER(client.getNick()));
		return ;
	}
	
	// 인자 개수가 틀린 경우
	if (cmds.size() != 2)
	{
		client.sendMsg(ServerMsg::NEEDMOREPARAMS(client.getNick(), cmds[0]));
		return ;
	}

	// 비밀번호가 틀린 경우
	if (cmds[1] != password)
	{
		client.sendMsg(ServerMsg::PASSWDMISMATCH());
		// 연결 종료
		std::map<int, Client>& clientList = Server::getClientList();
		clientList.erase(client.getFd());
	}

	client.setPassFlag(true);
}
