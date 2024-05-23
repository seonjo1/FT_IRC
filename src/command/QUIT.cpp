#include "../../include/core/Executor.hpp"
#include "../../include/core/Server.hpp"

void Executor::parseQUIT(std::vector<std::string>& cmds, std::string& msg)
{
	cmds.push_back("QUIT");
	int i = 4;
	int size = msg.size();

	// QUIT 다음 공백 다 넘기기
	while (i < size && msg[i] == ' ')
		i++;

	// ':'가 있으면 index 1 증가
	if (i < size && msg[i] == ':')
		i++;
	
	// 뒤에 내용이 있는경우 한번에 담기
	if (i < size)
		cmds.push_back(msg.substr(i));
	else
		cmds.push_back("");
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
		send(*iter, msg, str.size(), 0);

	// 나가는 클라이언트에게 메시지전달
	client.sendMsg(ServerMsg::QUITCHANNEL(client.getHostName(), client.getServerName(), cmds[1]));

	// 연결 종료 플래그 on
	client.setQuitFlag(true);
}
