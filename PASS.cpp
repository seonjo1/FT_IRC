#include "Executor.hpp"
#include "Server.hpp"

void Executor::PASS(Client& client, std::vector<std::string>& cmds)
{
	// 이미 PASS 인증이 끝난 유저
	if (client.getPassFlag())
	{
		client.sendMsg(ErrMsg::ALREADYREGISTER(client.getNick()));
		return ;
	}
	
	// 인자 개수가 틀린 경우
	if (cmds.size() != 2)
	{
		client.sendMsg(ErrMsg::NEEDMOREPARAMS(client.getNick(), cmds[0]));
		return ;
	}

	// 비밀번호가 틀린 경우
	if (cmds[1] != password)
	{
		client.sendMsg(ErrMsg::PASSWDMISMATCH());
		// 연결 종료
		Server::clientList.erase(client.getFd());
	}

	client.setPassFlag(true);
}