#include "../../include/core/Executor_bonus.hpp"
#include "../../include/core/Server_bonus.hpp"

// MODE 파싱
void Executor::parseMODE(std::vector<std::string>& cmds, std::string& msg)
{
	std::stringstream ss(msg);
	std::string cmd;
	while (ss >> cmd)
		cmds.push_back(cmd);
	
	int size = cmds.size();
	// 마지막 인자에 ':'가 있으면 제거
	if (cmds[size - 1][0] == ':')
		cmds[size - 1] = cmds[size - 1].substr(1);
}

// invite mode 변경
void inviteMode(Channel& channel, char sign, std::vector<std::string>& msgVector, char& plus)
{
	if (sign == '+') // +i
	{
		// 이미 +i 인경우 그냥 return
		if (channel.isInviteMode())
			return ;
		// +i 설정
		channel.setInviteMode(true);
	}
	else if (sign == '-') // -i
	{
		// 이미 -i 인경우 그냥 return
		if (!channel.isInviteMode())
			return ;
		// -i 설정
		channel.setInviteMode(false);
	}

	// msg 작성
	if (plus == sign)
		msgVector[0] += "i";
	else
		msgVector[0] += std::string(1, sign) + "i";
	plus = sign;
}

// topic mode 변경
void topicMode(Channel& channel, char sign, std::vector<std::string>& msgVector, char& plus)
{
	if (sign == '+') // +t
	{
		// 이미 +t 인경우 그냥 return
		if (channel.isTopicMode())
			return ;
		// +t 설정
		channel.setTopicMode(true);
	}
	else if (sign == '-') // -t
	{
		// 이미 -t 인경우 그냥 return
		if (!channel.isTopicMode())
			return ;
		// -t 설정
		channel.setTopicMode(false);
	}

	// msg 작성
	if (plus == sign)
		msgVector[0] += "t";
	else
		msgVector[0] += std::string(1, sign) + "t";
	plus = sign;
}

// key mode 변경
void keyMode(Client& client, Channel& channel, char sign, std::string param,
				std::vector<std::string>& msgVector, char& plus)
{
	// 모드에 따른 추가 인자 부족
	if (param.size() == 0)
	{
		client.addToSendBuf(ServerMsg::KEYMODENOPARAM(client.getNick(), channel.getName()));
		return ;
	}
	
	// 틀린 인자 처리 (명령어 별로)
	if (sign == '+') // +k 인 경우
	{
		// key parameter 유효성 검사
		if (Channel::isInvalidKey(param))
		{
			client.addToSendBuf(ServerMsg::UNKNOWNMODEKEY(client.getNick()));
			return ;
		}

		// 이미 +k 면 그냥 return
		if (channel.isKeyMode())
			return ;
		
		// +k 설정
		channel.setKeyMode(true, param);
	}
	else if (sign == '-') // -k 인 경우
	{
		// 이미 -k 면 그냥 return
		if (!channel.isKeyMode())
			return ;
		
		// key 가 틀리면 그냥 return
		if (param != channel.getKey())
			return ;
		
		channel.setKeyMode(false, param);
	}

	// msg 작성
	if (plus == sign)
		msgVector[0] += "k";
	else
		msgVector[0] += std::string(1, sign) + "k";

	plus = sign;
	msgVector.push_back(param);
}

// limit mode 변경
void limitMode(Client& client, Channel& channel, char sign, std::string param,
				std::vector<std::string>& msgVector, char& plus)
{
	if (sign == '+')
	{
		// 모드에 따른 추가 인자 부족
		if (param.size() == 0)
		{
			client.addToSendBuf(ServerMsg::LIMITMODENOPARAM(client.getNick(), channel.getName()));
			return ;
		}

		// limit param 유효성 검사
		if (Channel::isInvalidLimit(param))
		{
			client.addToSendBuf(ServerMsg::UNKNOWNMODELIMIT(client.getNick()));
			return ;
		}

		// 이미 +l 이면 return;
		if (channel.isLimitMode())
			return ;
		
		// +l 설정
		channel.setLimitMode(true, param);

		// msgVector에 param 추가
		msgVector.push_back(param);
	}
	else if (sign == '-')
	{
		// 이미 -l 인경우 그냥 return
		if (!channel.isLimitMode())
			return ;
		// -l 설정
		channel.setLimitMode(false, param);
	}

	// msg 작성
	if (plus == sign)
		msgVector[0] += "l";
	else
		msgVector[0] += std::string(1, sign) + "l";

	plus = sign;
}

// op mode 변경
void operatorMode(Client& client, Channel& channel, char sign, std::string param,
					std::vector<std::string>& msgVector, char& plus)
{
	// 모드에 따른 추가 인자 부족
	if (param.size() == 0)
	{
		client.addToSendBuf(ServerMsg::OPMODENOPARAM(client.getNick(), channel.getName()));
		return ;
	}

	// 없는 nick
	if (!Client::isNicknameInUse(param))
	{
		client.addToSendBuf(ServerMsg::NOSUCHNICK(client.getNick(), param));
		return ;
	}

	// 채널에 없는 nick
	if (!channel.doesClientExist(param))
	{
		return ;
	}

	if (sign == '+')
	{
		// 이미 +o 이면 return;
		if (channel.isOperator(param))
			return ;
		
		// +l 설정
		channel.setOPMode(true, param);
	}
	else if (sign == '-')
	{
		// 이미 -o 인경우 그냥 return
		if (!channel.isOperator(param))
			return ;
		// -o 설정
		channel.setOPMode(false, param);
	}

	// msg 작성
	if (plus == sign)
		msgVector[0] += "o";
	else
		msgVector[0] += std::string(1, sign) + "o";

	plus = sign;
	msgVector.push_back(param);
}

// MODE 실행
void Executor::MODE(Client& client, std::vector<std::string>& cmds)
{
	// 인자 부족
	if (cmds.size() < 2)
	{
		client.addToSendBuf(ServerMsg::NEEDMOREPARAMS(client.getNick(), cmds[0]));
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
		client.addToSendBuf(ServerMsg::NOSUCHCHANNEL(client.getNick(), cmds[1]));
		return ;
	}

	// 인자 2개면 그 방에 대한 정보 전송
	Channel& channel = Channel::getChannel(cmds[1]);
	if (cmds.size() == 2)
	{
		client.addToSendBuf(ServerMsg::CHANNELMODEIS(client.getNick(), channel.getName(), channel.getModeInfo(client.getNick())));
		client.addToSendBuf(ServerMsg::CHANNELINFO(client.getNick(), channel.getName(), channel.getCreatedTime()));
		return ;
	}

	// mode 명령어 파싱 2탄!
	std::vector<std::pair<std::string, std::string> > modeVector;
	int paramIdx = 3;
	int size = cmds[2].size();
	std::string sign = "+";
	for (int i = 0; i < size; i++)
	{
		std::string mode;
		std::string param;

		char c = cmds[2][i];

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
		else if (((c == 'l' && sign == "+") || c == 'k' || c == 'o') && paramIdx < static_cast<int>(cmds.size())) // 인자가 필요한 mode이면 param에 인자 추가
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
			client.addToSendBuf(ServerMsg::UNKNOWNMODE(client.getNick(), mode[1]));
			continue ;
		}

		// op 권한
		if (!channel.isOperator(client.getNick()))
		{
			client.addToSendBuf(ServerMsg::CHANOPRIVSNEEDEDMODEVERSION(client.getNick(), cmds[1], mode[1]));
			continue ;
		}


		//mode 별로 처리
		if (mode[1] == 'i') // invite mode
			inviteMode(channel, mode[0], msgVector, plus);
		else if (mode[1] == 't') // token mode
			topicMode(channel, mode[0], msgVector, plus);
		else if (mode[1] == 'k') // key mode
			keyMode(client, channel, mode[0], param, msgVector, plus);
		else if (mode[1] == 'l') // limit mode
			limitMode(client, channel, mode[0], param, msgVector, plus);
		else if (mode[1] == 'o') // operator mode
			operatorMode(client, channel, mode[0], param, msgVector, plus);
	}

	// 메시지가 있으면 전송
	if (msgVector[0].size() != 0)
	{
		// 마지막 인자 앞에 ':' 추가
		int size = msgVector.size();		
		msgVector[size - 1] = ":" + msgVector[size - 1];

		std::string modeInfo(msgVector[0]);
		for (int i = 1; i < size; i++)
			modeInfo += " " + msgVector[i];
		channel.addMsgToClientsSendBuf(ServerMsg::MODE(client.getNick(), client.getHostName(), client.getServerName(),
												channel.getName(), modeInfo));
	}
}