#include "../../include/core/Executor_bonus.hpp"
#include "../../include/core/Server_bonus.hpp"

// USER 파싱
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

	// 공백 다 pass
	while(i < size && msg[i] == ' ')
		i++;

	// ':'가 있으면 index 1 증가
	if (i < size && msg[i] == ':')
		i++;
	
	// 뒤에 내용이 있는경우 한번에 담기
	if (i < size)
		cmds.push_back(msg.substr(i));

}

// USER 실행
void Executor::USER(Client& client, std::vector<std::string>& cmds)
{
	// PASS 등록 안된 클라이언트
	if (!client.getPassFlag())
	{
		if (client.getNickFlag())
			client.addToSendBuf(ServerMsg::NOTREGISTERD(client.getNick()));
		else
			client.addToSendBuf(ServerMsg::NOTREGISTERD(""));
		// 연결 종료
		client.setQuitFlag(true);
		return ;
	}

	// 인자 개수가 다른 경우
	if (cmds.size() < 5)
	{
		if (client.getNickFlag())
			client.addToSendBuf(ServerMsg::NEEDMOREPARAMS(client.getNick(), cmds[0]));
		else
			client.addToSendBuf(ServerMsg::NEEDMOREPARAMS("", cmds[0]));
		return ;
	}

	// 이미 user 등록이 완료된 client
	if (client.getUserFlag())
	{
		if (client.getNickFlag())
			client.addToSendBuf(ServerMsg::ALREADYREGISTER(client.getNick()));
		else
			client.addToSendBuf(ServerMsg::ALREADYREGISTER(""));	
		return ;
	}

	client.setData(cmds[1], cmds[2], cmds[3], cmds[4]);
	client.setUserFlag(true);
}