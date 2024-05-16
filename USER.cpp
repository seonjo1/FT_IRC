#include "Executor.hpp"
#include "Server.hpp"

void Executor::USER(Client& client, std::vector<std::string>& cmds)
{
	// PASS 등록 안된 클라이언트
	if (!client.getPassFlag())
	{
		client.sendMsg(ErrMsg::NOTREGISTERD());
		// 연결 종료
		Server::clientList.erase(client.getFd());
	}

	// 인자 개수가 다른 경우
	if (cmds.size() != 2)
	{
		client.sendMsg(ErrMsg::NEEDMOREPARAMS(cmds[0]));
		return ;
	}

	// 이미 user 등록이 완료된 client
	if (client.getUserFlag())
	{
		client.sendMsg(ErrMsg::ALREADYREGISTER(cmds[0]));
		return ;
	}

	client.setUserFlag(true);
}