#include "../../include/utils/ServerMsg.hpp"
#include "../../include/core/Channel.hpp"
#include "../../include/core/Server.hpp"

// 이미 등록된 유저
std::string ServerMsg::ALREADYREGISTER(std::string nick)
{
	std::string msg;
	if (nick.size() > 0)
		msg = "462 " + nick + " :You may not reregister\r\n";
	else
		msg = "462 :You may not reregister\r\n";
	return (msg);
}

// 인자 부족
std::string ServerMsg::NEEDMOREPARAMS(std::string nick, std::string cmd)
{
	std::string msg;
	if (nick.size() > 0)
		msg = "461 " + nick + " " + cmd + " :Not enough parameters\r\n";
	else
		msg = "461 " + cmd + " :Not enough parameters\r\n";
	return (msg);
}

// 패스워드 틀림
std::string ServerMsg::PASSWDMISMATCH()
{
	std::string msg;
	msg = "464 :Password incorrect\r\n";
	return (msg);
}

// 등록이 되지 않은 유저
std::string ServerMsg::NOTREGISTERD(std::string nick)
{
	std::string msg;
	if (nick.size() > 0)
		msg = "451 " + nick + " :You have not registerd\r\n";
	else
		msg = "451 :You have not registerd\r\n";
	return (std::string());
}

// 닉네임이 인자로 들어오지 않음
std::string ServerMsg::NONICKNAMEGIVEN(std::string nick)
{
	std::string msg;
	if (nick.size() > 0)
		msg = "431 " + nick + " :No nickname given\r\n";
	else
		msg = "431 :No nickname given\r\n";
	return (msg);
}

// 올바르지 못한 형식의 닉네임
std::string ServerMsg::ERRONEUSNICKNAME(std::string nick, std::string errNick)
{
	std::string msg;
	if (nick.size() > 0)
		msg = "432 " + nick + " " + errNick + " :Erroneus nickname\r\n";
	else
		msg = "432 " + errNick + " :Erroneus nickname\r\n";
	return (msg);
}

// 이미 사용중인 닉네임
std::string ServerMsg::NICKNAMEINUSE(std::string nick, std::string errNick)
{
	std::string msg;
	msg = "433 " + nick + " " + errNick + " :Nickname is already in use\r\n";
	return (msg);
}

// 닉네임이 바뀜
std::string ServerMsg::NICKCHANGE(std::string oldNick, std::string hostname,
								  std::string servername, std::string newNick)
{
	std::string msg;
	msg = ":" + oldNick + "!" + hostname + "@" + servername + " NICK :" + newNick + "\r\n";
	return (msg);
}

// PING 메시지
std::string ServerMsg::PINGMSG(std::string serverIP)
{
	std::string msg;
	msg = "PING " + serverIP + " :" + serverIP + "\r\n";
	return (msg);
}

// 서버 인자 부족
std::string ServerMsg::NOORIGIN(std::string nick)
{
	std::string msg;
	msg = "409 " + nick + " :No origin specified\r\n";
	return (msg);
}

// 없는 서버
std::string ServerMsg::NOSUCHSERVER(std::string nick, std::string server)
{
	std::string msg;
	msg = "402 " + nick + " " + server + ":No such server\r\n";
	return (msg);
}

// 연결 확인용 welcome 멘트
std::string ServerMsg::WELCOME(std::string nick, std::string hostname,
							std::string servername)
{
	std::string msg;
	msg = ":" + Server::getIP() + " 001 " + nick + " :Welcome to the FT_IRC Network, "
		+ nick + "!" + hostname + "@" + servername + "\r\n";
	return (msg);
}

/*
	MOTD(Message Of The Day)가 없다는 메시지.
	여기서 MOTD는 연결 완료시 서버로 부터 받는 환영 메시지이다.
*/
std::string ServerMsg::NOMOTD(std::string nick)
{
	std::string msg;
	msg = ":" + Server::getIP() + " 422 " + nick + " :MOTD File is missing\r\n"; 
	return (msg);
}

// 올바르지 못한 형식의 채널 이름
std::string ServerMsg::INVALIDCHANNEL(std::string nick, std::string server)
{
	std::string msg;
	msg = "476 " + nick + " " + server + " :Invalid channel name\r\n";
	return (msg);
}

// 토픽 내용
std::string ServerMsg::TOPIC(std::string nick, std::string channel, 
						std::string topic)
{
	std::string msg;
	msg = "332 " + nick + " " + channel + " :" + topic + "\r\n";
	return (msg);
}

// 토픽 작성자, 생성 시간
std::string ServerMsg::TOPICINFO(std::string nick, std::string channel,
							std::string topicWriter, int time)
{
	std::stringstream ss;
	ss << time;
	std::string msg;
	msg = "333 " + nick + " " + channel + " " + topicWriter + " :" + ss.str() + "\r\n";
	return (msg);
}

// 채널 key가 틀려 참가 불가
std::string ServerMsg::BADCHANNELKEY(std::string nick, std::string channel)
{
	std::string msg;
	msg = "475 " + nick + " " + channel + " :Cannot join channel (+k)\r\n";
	return (msg);	
}

// 초대로만 들어올 수 있는 방이라 참가 불가
std::string ServerMsg::INVITEONLYCHAN(std::string nick, std::string channel)
{
	std::string msg;
	msg = "473 " + nick + " " + channel + " :Cannot join channel (+i)\r\n";
	return (msg);
}

// 풀방이라 참가 불가
std::string ServerMsg::CHANNELISFULL(std::string nick, std::string channel)
{
	std::string msg;
	msg = "471 " + nick + " " + channel + " :Cannot join channel (+l)\r\n";
	return (msg);
}

// 참여 완료 메시지
std::string ServerMsg::JOIN(std::string nick, std::string hostname,
						std::string servername, std::string channel)
{
	std::string msg;
	msg = ":" + nick + "!" + hostname + "@" + servername + " JOIN " + channel + "\r\n";
	return (msg);
}

// PONG 메시지
std::string ServerMsg::PONG(std::string cmd1, std::string cmd2)
{
	std::string msg;
	if (cmd2.size() == 0)
		msg = "PONG " + cmd1 + "\r\n";
	else
		msg = "PONG " + cmd1 + " " + cmd2 + "\r\n";
	return (msg);
}

// 존재하지 않는 채널
std::string ServerMsg::NOSUCHCHANNEL(std::string nick, std::string channel)
{
	std::string msg;
	msg = "403 " + nick + " " + channel + " :No such channel\r\n";
	return (msg);
}

// 현재 해당 채널에 존재하지 않음
std::string ServerMsg::NOTONCHANNEL(std::string nick, std::string channel)
{
	std::string msg;
	msg = "442 " + nick + " " + channel + " :You're not on that channel\r\n";
	return (msg);
}

// PART 메시지
std::string ServerMsg::PART(std::string nick, std::string hostname, std::string servername,
							std::string channel, std::string message)
{
	std::string msg;

	if (message.size() == 0)
		msg = ":" + nick + "!" + hostname + "@" + servername + " PART :" + channel + "\r\n";
	else
		msg = ":" + nick + "!" + hostname + "@" + servername + " PART " + channel + " :" + message + "\r\n";
	return (msg);
}

// QUIT 메시지 (같은 채널에 있던 사람들에게 보내는 메시지)
std::string ServerMsg::QUITMSGTOCHANNEL(std::string nick, std::string hostname,
									std::string servername, std::string message)
{
	std::string msg;
	msg = ":" + nick + "!" + hostname + "@" + servername + " QUIT :Quit: " + message + "\r\n";
	return (msg);
}

// QUIT 메시지 (QUIT 명령어 사용자에게 보내는 메시지)
std::string ServerMsg::QUITCHANNEL(std::string hostname, std::string servername, std::string message)
{
	std::string msg;
	msg = "ERROR :Closing link: (" + hostname + "@" + servername + ") [Quit: " + message + "]\r\n";
	return (msg);
}

// 받는 사람을 인자로 주지 않음
std::string ServerMsg::NORECIPIENT(std::string nick, std::string command)
{
	std::string msg;
	msg = "411 " + nick + " :No recipient given (" + command + ")\r\n";
	return (msg);
}

// 보낼 메시지를 인자로 주지 않음
std::string ServerMsg::NOTEXTTOSEND(std::string nick)
{
	std::string msg;
	msg = "412 " + nick + " :No text to send\r\n";
	return (msg);
}

// 존재하지 않는 Nickname
std::string ServerMsg::NOSUCHNICK(std::string clientNick, std::string findNick)
{
	std::string msg;
	msg = "401 " + clientNick + " " + findNick + " :No such nick\r\n";
	return (msg);
}

// 개인 메시지 전송
std::string ServerMsg::PRIVMSG(std::string nick, std::string hostname, 
							   std::string servername,	std::string receiver,
							   std::string message)
{
	std::string msg;
	msg = ":" + nick + "!" + hostname + "@" + servername + " PRIVMSG " + receiver + " :" + message + "\r\n";
	return (msg);
}

// 채널 참가자 명단
std::string ServerMsg::NAMREPLY(std::string nick, Channel& channel)
{
	std::string msg;
	msg = "353 " + nick + " = " + channel.getName() + " :";
	
	// operator list 명단 적기
	std::vector<Client*>& opList = channel.getOpList();
	int opSize = opList.size();
	for (int i = 0; i < opSize; i++)
		msg += "@" + opList[i]->getNick() + " ";
	
	// 일반 참여자 list 명단 적기
	std::vector<Client*>& joinList = channel.getJoinList();
	int joinSize = joinList.size();
	for (int i = 0; i < joinSize; i++)
		msg += joinList[i]->getNick() + " ";
	
	// 마지막 공백 제거하고 CRLF 추가
	Bot& bot = channel.getBot();
	msg += bot.getName() + "\r\n";
	return (msg);
}

// 참가자 명단 끝을 알림
std::string ServerMsg::ENDOFNAMES(std::string nick, std::string channel)
{
	std::string msg;
	msg = "366 " + nick + " " + channel + " :End of /NAMES list\r\n";
	return (msg);
}

// target Nickname이 해당 채널에 존재하지 않음
std::string ServerMsg::USERNOTINCHANNEL(std::string nick, std::string targetNick, std::string channel)
{
	std::string msg;
	msg = "441 " + nick + " " + targetNick + " " + channel + " :They are not on that channel\r\n";
	return (msg);
}

// operator 권한이 없음
std::string ServerMsg::CHANOPRIVSNEEDED(std::string nick, std::string channel)
{
	std::string msg;
	msg = "482 " + nick + " " + channel + " :You must be a channel operator\r\n";
	return (msg);
}

// KICK 메시지
std::string ServerMsg::KICK(std::string nick, std::string hostname, std::string servername,
							std::string channel, std::string targetNick, std::string message)
{
	std::string msg;
	msg = ":" + nick + "!" + hostname + "@" + servername + " KICK " + channel + " " + targetNick + " :" + message + "\r\n";
	return (msg);
}

// 이미 해당 채널에 참여해있음
std::string ServerMsg::USERONCHANNEL(std::string nick, std::string user, std::string channel)
{
	std::string msg;
	msg = "443 " + nick + " " + user + " " + channel + " :is already on channel\r\n";
	return (msg);
}

// 초대 메시지 (초대한 사람에게 보내는 메시지)
std::string ServerMsg::INVITE(std::string nick, std::string targetNick, std::string channel)
{
	std::string msg;
	msg = "341 " + nick + " " + targetNick + " :" + channel + "\r\n";
	return (msg);
}

// 초대 메시지 (초대 받은 사람에게 보내는 메시지)
std::string ServerMsg::INVITETOTARGET(std::string nick, std::string hostname, std::string servername,
									std::string targetNick, std::string channel)
{
	std::string msg;
	msg = ":" + nick + "!" + hostname + "@" + servername + " INVITE " + targetNick + " :" + channel + "\r\n";
	return (msg); 
}

// 초대 메시지 (초대를 한 채널에 보내는 메시지)
std::string ServerMsg::INVITENOTICE(std::string nick, std::string channel, std::string targetNick)
{
	std::string msg;
	msg = "NOTICE " + channel + " :*** " + nick + " invited " + targetNick + " into the channel\r\n";
	return (msg);
}

// 토픽이 없다는 메시지
std::string ServerMsg::NOTOPIC(std::string nick, std::string channel)
{
	std::string msg;
	msg = "331 " + nick + " " + channel + " :No toopic is set\r\n";
	return (msg);
}

// 토픽을 작성할 권한이 없음
std::string ServerMsg::CHANOPRIVSNEEDEDTOPICVERSION(std::string nick, std::string channel)
{
	std::string msg;
	msg = "482 " + nick + " " + channel + " :You do not have access to change the topic on this channel\r\n";
	return (msg);
}

// 토픽이 변경사항 전달
std::string ServerMsg::TOPICCHANGE(std::string nick, std::string hostname, std::string servername,
									std::string channel, std::string topic)
{
	std::string msg;
	msg = ":" + nick + "!" + hostname + "@" + servername + " TOPIC " + channel + " :" + topic + "\r\n";
	return (msg);
}

// 해당 mode를 실행하기 위한 operator 권한이 없음
std::string ServerMsg::CHANOPRIVSNEEDEDMODEVERSION(std::string nick, std::string channel, char mode)
{
	std::string msg;
	msg = "482 " + nick + " " + channel + " :You must have channel op access or above to set channel mode " + mode + "\r\n";
	return (msg);
}

// 없는 채널 mode
std::string ServerMsg::UNKNOWNMODE(std::string nick, char mode)
{
	std::string msg;
	msg = "472 " + nick + " " + mode + " :is an unknown mode character\r\n";
	return (msg);
}

// 현재 채널 모드 상태를 전달
std::string ServerMsg::CHANNELMODEIS(std::string nick, std::string channel, std::string modeInfo)
{
	std::string msg;
	msg = "324 " + nick + " " + channel + " " + modeInfo + "\r\n";
	return (msg);
}

// 채널 이름, 생성 시간 전달
std::string ServerMsg::CHANNELINFO(std::string nick, std::string channel, int channelCreatedTime)
{
	std::stringstream ss;
	ss << channelCreatedTime;
	std::string msg;
	msg = "329 " + nick + " " + channel + " :" + ss.str() + "\r\n";
	return (msg);
}

// 채널 모드 k에 인자를 안 줌
std::string ServerMsg::KEYMODENOPARAM(std::string nick, std::string channel)
{
	std::string msg;
	msg = "696 " + nick + " " + channel + " k * :You must specify a parameter for the key mode. Syntax: <key>.\r\n";
	return (msg);
}

// 채널 모드 +l에 인자를 안 줌
std::string ServerMsg::LIMITMODENOPARAM(std::string nick, std::string channel)
{
	std::string msg;
	msg = "696 " + nick + " " + channel + " l * :You must specify a parameter for the limit mode. Syntax: <limit>.\r\n";
	return (msg);
}

// 채널 모드 o에 인자를 안 줌
std::string ServerMsg::OPMODENOPARAM(std::string nick, std::string channel)
{
	std::string msg;
	msg = "696 " + nick + " " + channel + " o * :You must specify a parameter for the op mode. Syntax: <nick>.\r\n";
	return (msg);
}

// 채널 모드 k의 key가 올바르지 못한 형식임
std::string ServerMsg::UNKNOWNMODEKEY(std::string nick)
{
	std::string msg;
	msg = "472 " + nick + " :Invalid key\r\n";
	return (msg);
}

// 채널 모드 l의 limit이 올바르지 못한 형식임
std::string ServerMsg::UNKNOWNMODELIMIT(std::string nick)
{
	std::string msg;
	msg = "472 " + nick + " :Invalild limit\r\n";
	return (msg);
}

// 채널 모드의 변경 사항 전달
std::string ServerMsg::MODE(std::string nick, std::string hostname, std::string servername,
							std::string channel, std::string modeInfo)
{
	std::string msg;
	msg = ":" + nick + "!" + hostname + "@" + servername + " MODE " + channel + " " + modeInfo + "\r\n";
	return (msg);
}

// chat bot이 채널에 메시지 전달
std::string ServerMsg::BOTPRIVMSG(std::string name, std::string channel, std::string message)
{
	std::string msg;
	msg = ":" + name + " PRIVMSG " + channel + " :" + message + "\r\n";
	return (msg);
}