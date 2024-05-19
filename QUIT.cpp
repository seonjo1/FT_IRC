#include "Executor.hpp"
#include "Server.hpp"

void Executor::parseQUIT(std::vector<std::string>& cmds, std::string& msg)
{
	cmds.push_back("QUIT");
	int i = 4;
	int size = msg.size();

	// QUIT 다음 공백 다 넘기기
	while (i < size && msg[i] == ' ')
		i++;
	
	// 공백이 끝난 다음 문자가 ':'면
	if (i + 1 >= size || msg[i] != ':')
		return ;
	if (i + 1 == size)
		cmds.push_back("");
	else
		cmds.push_back(msg.substr(i + 1));
}

void Executor::QUIT(Client& client, std::vector<std::string>& cmds)
{
	// message가 없으면 default message인 nick으로 message 추가
	if (cmds.size() == 1)
		cmds.push_back(client.getNick());
	
	// 클라이언트가 참여한 모든 채널에 있는 사람에게 메시지 전달 (본인 빼고)
	// 모든 채널에 있는 클라이언트들의 fd 수집
	std::set<int> set;
	std::vector<Channel*> joinedChannels = client.getJoinedChannels();
	for (int i = 0; i < static_cast<int>(joinedChannels.size()); i++)
		joinedChannels[i]->fillSetWithFd(set);
	set.erase(client.getFd()); // 본인 빼고

	// 메시지 만들기
	std::string str = ServerMsg::QUITMSGTOCHANNEL(client.getNick(), client.getHostName(),
											 client.getServerName(), cmds[1]);
	const char *msg = str.c_str();
	
	// 메시지 전달
	std::set<int>::iterator iter;
	for (iter = set.begin(); iter != set.end(); iter++)
	{
		if (send(*iter, msg, str.size(), 0) == -1)
			std::cout << "send fail\n";
	}

	// 나가는 클라이언트에게 메시지전달
	client.sendMsg(ServerMsg::QUITCHANNEL(client.getHostName(), client.getServerName(), cmds[1]));

	// 연결 종료 플래그 on
	client.setQuitflag(true);
}
