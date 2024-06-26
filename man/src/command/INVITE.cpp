#include "../../include/core/Executor.hpp"
#include "../../include/core/Server.hpp"

// INVITE 파싱
void Executor::parseINVITE(std::vector<std::string>& cmds, std::string& msg)
{
	std::stringstream ss(msg);
	std::string cmd;
	while (ss >> cmd)
		cmds.push_back(cmd);
}

// INVITE 실행
void Executor::INVITE(Client& client, std::vector<std::string>& cmds)
{
	// 인자 검사
	if (cmds.size() < 3)
	{
		client.addToSendBuf(ServerMsg::NEEDMOREPARAMS(client.getNick(), cmds[0]));
		return ;
	}

	// 없는 채널
	if (!Channel::isChannelInUse(cmds[2]))
	{
		client.addToSendBuf(ServerMsg::NOSUCHCHANNEL(client.getNick(), cmds[2]));
		return ;
	}

	// 없는 유저
	if (!Client::isNicknameInUse(cmds[1]))
	{
		client.addToSendBuf(ServerMsg::NOSUCHNICK(client.getNick(), cmds[1]));
		return ;
	}

	// 내가 채널에 없을때
	Channel& channel = Channel::getChannel(cmds[2]);
	if (!channel.doesClientExist(client.getNick()))
	{
		client.addToSendBuf(ServerMsg::NOTONCHANNEL(client.getNick(), cmds[2]));
		return ;
	}

	// 유저가 이미 존재
	if (channel.doesClientExist(cmds[1]))
	{
		client.addToSendBuf(ServerMsg::USERONCHANNEL(client.getNick(), cmds[1], cmds[2]));
		return ;
	}

	// 오퍼레이터 x
	if (!channel.isOperator(client.getNick()))
	{
		client.addToSendBuf(ServerMsg::CHANOPRIVSNEEDED(client.getNick(), cmds[2]));
		return ;
	}

	// 초대 리스트에 추가
	channel.addToInviteList(cmds[1]);

	// 초대한 클라이언트에게 초대 완료 알림
	client.addToSendBuf(ServerMsg::INVITE(client.getNick(), cmds[1], cmds[2]));

	// target에게 초대 메시지 전달
	Client& target = Client::getClient(cmds[1]);
	target.addToSendBuf(ServerMsg::INVITETOTARGET(client.getNick(), client.getHostName(),
												client.getServerName(),	cmds[1], cmds[2]));
	// 채널 사람들에게 초대 사실 전송
	channel.addMsgToClientsSendBuf(ServerMsg::INVITENOTICE(client.getNick(), cmds[2], cmds[1]), client);
}
