#include "Executor.hpp"
#include "Server.hpp"

void Executor::parseMODE(std::vector<std::string>& cmds, std::string& msg)
{
	std::stringstream ss(msg);
	std::string cmd;
	while (ss >> cmd)
		cmds.push_back(cmd);
	
	// 마지막 인자에 ':'가 있으면 제거
	if (cmds.back()[0] == ':')
		cmds.back() = cmds.back().substr(1);
}

void inviteMode(Client& client, Channel& channel, char sign, char mode,
					std::vector<std::string>& msgVector, char& plus)
{
	if (sign == '+') // +i
	{
		if (channel.isInviteMode()) // 이미 +i 인경우 그냥 return
			return ;
		// +i 설정
		channel.setInviteMode(true);
	}
	else if (sign == '-') // -i
	{
		if (!channel.isInviteMode()) // 이미 -i 인경우 그냥 return
			return ;
		// -i 설정
		channel.setInviteMode(false);
	}

	// msg 작성
	if (plus == sign)
		msgVector[0] += "i";
	else
		msgVector[0] += sign + "i";
	plus = sign;
}


void tokenMode(Client& client, Channel& channel, char sign, char mode,
				std::vector<std::string>& msgVector, char& plus)
{
	if (sign == '+') // +t
	{
		if (channel.isTopicMode()) // 이미 +t 인경우 그냥 return
			return ;
		// +t 설정
		channel.setTopicMode(true);
	}
	else if (sign == '-') // -t
	{
		if (!channel.isTopicMode()) // 이미 -t 인경우 그냥 return
			return ;
		// -t 설정
		channel.setTopicMode(false);
	}

	// msg 작성
	if (plus == sign)
		msgVector[0] += "t";
	else
		msgVector[0] += sign + "t";
	plus = sign;
}


void keyMode(Client& client, Channel& channel, char sign, char mode, std::string param,
				std::vector<std::string>& msgVector, char& plus)
{
	// 모드에 따른 추가 인자 부족

	// 틀린 인자 처리 (명령어 별로)
}


void operatorMode(Client& client, Channel& channel, char sign, char mode, std::string param,
					std::vector<std::string>& msgVector, char& plus)
{
	// 모드에 따른 추가 인자 부족

	// 틀린 인자 처리 (명령어 별로)
	
}


void limitMode(Client& client, Channel& channel, char sign, char mode, std::string param,
				std::vector<std::string>& msgVector, char& plus)
{
	// 모드에 따른 추가 인자 부족

	// 틀린 인자 처리 (명령어 별로)
	
}

void Executor::MODE(Client& client, std::vector<std::string>& cmds)
{
	// 인자 부족
	if (cmds.size() < 2)
	{
		client.sendMsg(ServerMsg::NEEDMOREPARAMS(client.getNick(), cmds[0]));
		return ;
	}

	// 유저모드 명령어 무시
	if (cmds[1][0] != '#' && cmds[1][0] != '&')
	{
		return ;
	}

	// 없는 채널
	if (!Channel::isChannelInUse(cmds[1]))
	{
		client.sendMsg(ServerMsg::NOSUCHCHANNEL(client.getNick(), cmds[1]));
		return ;
	}

	// 인자 2개면 그 방에 대한 정보 전송
	if (cmds.size() == 2)
	{
		Channel& channel = Channel::getChannel(cmds[1]);
		client.sendMsg(ServerMsg::CHANNELMODEIS(client.getNick(), channel.getName(), channel.getModeInfo(client.getNick())));
		client.sendMsg(ServerMsg::CHANNELINFO(client.getNick(), channel.getName(), channel.getCreatedTime()));
		return ;
	}

	// mode 명령어 파싱 2탄!
	std::vector<std::pair<std::string, std::string> > modeVector;
	int paramIdx = 2;
	int size = cmds[1].size();
	std::string sign = "+";
	for (int i = 0; i < size; i++)
	{
		std::string mode;
		std::string param;

		char c = cmds[1][i];

		if (c == '+') // '+'면 sign +로 설정
		{
			sign = c;
			continue ;
		}
		else if (c == '-') // '-'면 sign -로 설정
		{
			sign = c;
			continue ;
		}
		else if ((c == 'l' || c == 'k' || c == 'o') && paramIdx < static_cast<int>(cmds.size())) // 인자가 필요한 mode이면 param에 인자 추가
		{	
			param = cmds[paramIdx++]; // param 추가하고 paramIdx 1 증가
		}

		mode = sign + c; // mode = 부호 + 모드 
		modeVector.push_back(std::make_pair(mode, param)); // mode와 param 짝지어서 벡터에 삽입
	}

	std::vector<std::string> msgVector;
	msgVector.push_back("");
	char plus = 0;
	//LOOP (i, t, o, k, l) 짝지은 pair를 순회하며 실행
	for (int i = 0; i < static_cast<int>(modeVector.size()); i++)
	{
		std::string mode = modeVector[i].first;
		std::string param = modeVector[i].second;

		// 없는 모드
		if (mode[1] != 'i' && mode[1] != 't' && mode[1] != 'k' &&
			mode[1] != 'o' && mode[1] != 'l')
		{
			client.sendMsg(ServerMsg::UNKNOWNMODE(client.getNick(), mode[1]));
			continue ;
		}

		// op 권한
		Channel& channel = Channel::getChannel(cmds[1]);
		if (!channel.isOperator(client.getNick()))
		{
			client.sendMsg(ServerMsg::CHANOPRIVSNEEDEDMODEVERSION(client.getNick(), cmds[1], mode[1]));
			continue ;
		}


		//mode 별로 처리
		if (mode[1] == 'i') // invite mode
			inviteMode(client, channel, mode[0], mode[1], msgVector, plus);
		else if (mode[1] == 't') // token mode
			tokenMode(client, channel, mode[0], mode[1], msgVector, plus);
		else if (mode[1] == 'k') // key mode
			keyMode(client, channel, mode[0], mode[1], param, msgVector, plus);
		else if (mode[1] == 'o') // operator mode
			operatorMode(client, channel, mode[0], mode[1], param, msgVector, plus);
		else if (mode[1] == 'l') // limit mode
			limitMode(client, channel, mode[0], mode[1], param, msgVector, plus);
	}

}