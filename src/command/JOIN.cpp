#include "../../include/core/Executor.hpp"
#include "../../include/core/Server.hpp"

void Executor::parseJOIN(std::vector<std::string>& cmds, std::string& msg)
{
	std::stringstream ss(msg);
	std::string cmd;
	while (ss >> cmd)
		cmds.push_back(cmd);
}

void Executor::JOIN(Client& client, std::vector<std::string>& cmds)
{

	// 매개변수가 부족한 경우
	if (cmds.size() < 2)
	{
		client.sendMsg(ServerMsg::NEEDMOREPARAMS(client.getNick(), cmds[0]));
		return ;
	}

	// join 명령어 파싱 2탄!
	// 1번째 인자의 ','를 ' '로 변경
	for (int i = 0; i < static_cast<int>(cmds[1].size()); i++)
	{
		if (cmds[1][i] == ',')
			cmds[1][i] = ' ';
	}
	// 첫번째인자를 ' '기준으로 나누면서 나머지 인자들을 짝으로 맞춰줌
	std::vector<std::pair<std::string, std::string> > cmdPairVector;
	std::stringstream ss(cmds[1]);
	int idx = 2;
	std::string str;
	while (ss >> str)
	{
		std::string pair = "";
		if (idx < static_cast<int>(cmds.size()))
			pair = cmds[idx++];
		cmdPairVector.push_back(std::make_pair(str, pair));
	}

	for (int i = 0; i < static_cast<int>(cmdPairVector.size()); i++)
	{
		std::string room = cmdPairVector[i].first;
		std::string param = cmdPairVector[i].second;

		// 잘못된 채널 이름인 경우
		if (Channel::isInvalidChannelName(room))
		{
			client.sendMsg(ServerMsg::INVALIDCHANNEL(client.getNick(), room));
			continue;
		}
		// 이미 참여한 채널인 경우
		if (client.isClientMemberOfChannel(room))
		{
			continue;
		}

		// 채널에 참가
		Channel::joinChannel(client, room, param);
	}
}