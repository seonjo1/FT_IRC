#include "Executor.hpp"
#include "Server.hpp"

void Executor::parseUSER(std::vector<std::string>& cmds, std::string& msg)
{
	int size = msg.size();
	int argNum = 0;
	int i = 0;

	// 공백 기준으로 앞에 인자 4개 구분
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

	// 5번째 인자는 ':'으로 시작하면 인정
	while(i < size && msg[i] != ':')
	{
		// 5번째 인자가 ':'으로 시작안하는 애면 오류
		if (msg[i] != ' ')
		{
			cmds.push_back("error");
			cmds.push_back("error");
			return ;
		}
		i++;
	}

	// 5번째 인자 추가
	if (++i < size)
		cmds.push_back(msg.substr(i));

}

void Executor::USER(Client& client, std::vector<std::string>& cmds)
{
	// PASS 등록 안된 클라이언트
	if (!client.getPassFlag())
	{
		if (client.getNickFlag())
			client.sendMsg(ServerMsg::NOTREGISTERD(client.getNick()));
		else
			client.sendMsg(ServerMsg::NOTREGISTERD(""));
		// 연결 종료
		std::map<int, Client>& clientList = Server::getClientList();
		clientList.erase(client.getFd());
	}

	// 인자 개수가 다른 경우
	if (cmds.size() != 5)
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