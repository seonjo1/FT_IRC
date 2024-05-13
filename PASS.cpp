#include "Executor.hpp"

void Executor::PASS(Client& client, std::vector<std::string>& cmds)
{
	int fd = client.getFd();
	// 이미 PASS 인증이 끝난 유저
	if (client.getPassFlag())
	{
		sendMsg(fd, ErrMsg::ALREADYREGISTER(client.getNick()));
		return ;
	}
	
	// 인자 개수가 틀린 경우
	if (cmds.size() != 2)
	{
		sendMsg(fd, ErrMsg::NEEDMOREPARAMS(client.getNick(), cmds[0]));
		return ;
	}

	// 비밀번호가 틀린 경우
	if (cmds[1] != password)
	{
		sendMsg(fd, ErrMsg::PASSWDMISMATCH());
		// 연결 종료
		kq->removeSocket(fd);
		clientList->erase(fd);
	}

	client.setPassFlag(true);
}