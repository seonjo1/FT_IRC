#include "Executor.hpp"
#include "Server.hpp"

void Executor::parsePONG(std::vector<std::string>& cmds, std::string& msg)
{
	int size = msg.size();
	int argNum = 0;
	int i = 0;

	// 공백 기준으로 앞에 인자 2개 구분
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

	// 3번째 인자는 ':'으로 시작하면 인정
	while(i < size && msg[i] != ':')
	{
		// 3번째 인자가 ':'으로 시작안하는 애면 오류
		if (msg[i] != ' ')
		{
			cmds.push_back("error");
			cmds.push_back("error");
			return ;
		}
		i++;
	}

	// 3번째 인자 추가
	if (++i < size)
		cmds.push_back(msg.substr(i));
}

void Executor::PONG(Client& client, std::vector<std::string>& cmds)
{
	if (cmds.size() == 1) // 인자 부족
		client.sendMsg(ServerMsg::NOORIGIN(client.getNick()));
	else if (cmds[1] != Server::getIP()) // 서버 IP 답장이 틀림
		client.sendMsg(ServerMsg::NOSUCHSERVER(client.getNick(), cmds[1]));
	else if (cmds.size() < 4)
	{
		// 연결 성공했으므로 welcome msg 전송
		client.sendMsg(ServerMsg::WELCOME(client.getNick(), client.getHostName(), client.getServerName()));
		client.sendMsg(ServerMsg::NOMOTD(client.getNick()));
		return ;
	}
	// PONG 메시지 오류시 연결 종료
	client.setQuitFlag(true);
}