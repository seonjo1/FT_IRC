#include "../../include/core/Executor_bonus.hpp"
#include "../../include/core/Server_bonus.hpp"

// PASS 파싱
void Executor::parsePASS(std::vector<std::string>& cmds, std::string& msg)
{
	std::stringstream ss(msg);
	std::string cmd;
	while (ss >> cmd)
		cmds.push_back(cmd);
}

// PASS 실행
void Executor::PASS(Client& client, std::vector<std::string>& cmds)
{
	// 이미 PASS 인증이 끝난 유저
	if (client.getPassFlag())
	{
		client.addToSendBuf(ServerMsg::ALREADYREGISTER(client.getNick()));
		return ;
	}
	
	// 인자 개수가 틀린 경우
	if (cmds.size() < 2)
	{
		client.addToSendBuf(ServerMsg::NEEDMOREPARAMS(client.getNick(), cmds[0]));
		return ;
	}

	// 비밀번호가 틀린 경우
	if (cmds[1] != password)
	{
		client.addToSendBuf(ServerMsg::PASSWDMISMATCH());
		client.setQuitFlag(true);
		return ;
	}

	// 성공시 PassFlag 설정
	client.setPassFlag(true);
}
