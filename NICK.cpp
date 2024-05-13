#include "Executor.hpp"

void Executor::NICK(Client& client, std::vector<std::string>& cmds)
{
	int fd = client.getFd();
	if (!client.getNickFlag())
	{
		// PASS 등록 안된 클라이언트
		if (!client.getPassFlag())
		{
			sendMsg(fd, ErrMsg::NOTREGISTERD());
			// 연결 종료
			kq->removeSocket(fd);
			clientList->erase(fd);			
		}

		// 인자 개수가 다른 경우
		if (cmds.size() == 1)
		{
			sendMsg(fd, ErrMsg::NONICKNAMEGIVEN());
			return ;
		}
		else if (cmds.size() != 2)
		{
			sendMsg(fd, ErrMsg::NEEDMOREPARAMS(cmds[0]));
			return ;
		}

		// 유효하지 않은 nickname
		if (isInvalidNick(cmds[1]))
		{
			sendMsg(fd, ErrMsg::ERRONEUSNICKNAME(cmds[1]));
			return ;
		}
		// 이미 존재하는 nickname
		if (isNicknameInUse(cmds[1]))
		{
			sendMsg(fd, ErrMsg::NICKNAMEINUSE(cmds[1]));
			return ;
		}

		// 등록 가능한 NICK
		client.setNick(cmds[1]);
		addNick(cmds[1]);
		client.setNickFlag(true);
	}
	else
	{
		// 인자 개수가 다른 경우
		if (cmds.size() == 1)
		{
			sendMsg(fd, ErrMsg::NONICKNAMEGIVEN());
			return ;
		}
		else if (cmds.size() != 2)
		{
			sendMsg(fd, ErrMsg::NEEDMOREPARAMS(client.getNick(), cmds[0]));
			return ;
		}

		// 유효하지 않은 nickname
		if (isInvalidNick(cmds[1]))
		{
			sendMsg(fd, ErrMsg::ERRONEUSNICKNAME(client.getNick(),cmds[1]));
			return ;
		}
		// 이미 존재하는 nickname
		if (isNicknameInUse(cmds[1]))
		{
			sendMsg(fd, ErrMsg::NICKNAMEINUSE(client.getNick(),cmds[1]));
			return ;
		}

		// nickList 수정
		addNick(cmds[1]);
		removeNick(client.getNick());

		// channel 수정
		std::vector<Channel>& channelVec = client.getChannelVec();
		for (std::vector<Channel>::iterator iter = channelVec.begin(); iter != channelVec.end(); iter++)
			iter->changeNickInChannel(client.getNick(), cmds[1]);
		
		// 클라이언트 nick 수정
		client.setNick(cmds[1]);
	}
}