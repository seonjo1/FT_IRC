#include "Executor.hpp"
#include "Server.hpp"

void Executor::parseNICK(std::vector<std::string>& cmds, std::string& msg)
{
	std::stringstream ss(msg);
	std::string cmd;
	while (ss >> cmd)
		cmds.push_back(cmd);
}

void Executor::NICK(Client& client, std::vector<std::string>& cmds)
{
	if (!client.getNickFlag())
	{
		// PASS 등록 안된 클라이언트
		if (!client.getPassFlag())
		{
			client.sendMsg(ServerMsg::NOTREGISTERD(client.getNick()));
			client.setErrflag(true);
			return ;
		}

		// nickname 인자가 주어지지 않은 경우
		if (cmds.size() == 1)
		{
			client.sendMsg(ServerMsg::NONICKNAMEGIVEN(client.getNick()));
			return ;
		}
		
		// 인자 개수가 다른 경우
		if (cmds.size() != 2)
		{
			client.sendMsg(ServerMsg::NEEDMOREPARAMS(client.getNick(), cmds[0]));
			return ;
		}

		// 유효하지 않은 nickname
		if (Client::isInvalidNick(cmds[1]))
		{
			client.sendMsg(ServerMsg::ERRONEUSNICKNAME(client.getNick(), cmds[1]));
			return ;
		}

		// 이미 존재하는 nickname
		if (Client::isNicknameInUse(cmds[1]))
		{
			client.sendMsg(ServerMsg::NICKNAMEINUSE(cmds[1], cmds[1]));
			return ;
		}

		// 등록 가능한 NICK이므로 NICK 설정
		client.addNick(cmds[1]);
		client.setNickFlag(true);
	}
	else
	{
		// 인자 개수가 다른 경우
		if (cmds.size() == 1)
		{
			client.sendMsg(ServerMsg::NONICKNAMEGIVEN(client.getNick()));
			return ;
		}
		else if (cmds.size() != 2)
		{
			client.sendMsg(ServerMsg::NEEDMOREPARAMS(client.getNick(), cmds[0]));
			return ;
		}

		// 유효하지 않은 nickname
		if (Client::isInvalidNick(cmds[1]))
		{
			client.sendMsg(ServerMsg::ERRONEUSNICKNAME(client.getNick(),cmds[1]));
			return ;
		}

		// 이미 존재하는 nickname
		if (Client::isNicknameInUse(cmds[1]))
		{
			client.sendMsg(ServerMsg::NICKNAMEINUSE(client.getNick(),cmds[1]));
			return ;
		}

		// nickList 수정
		client.changeNick(cmds[1]);
	}
}