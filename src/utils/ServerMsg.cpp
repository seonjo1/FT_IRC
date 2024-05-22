#include "../../include/utils/ServerMsg.hpp"
#include "../../include/core/Channel.hpp"
#include "../../include/core/Server.hpp"

std::string ServerMsg::ALREADYREGISTER(std::string nick)
{
	std::string msg;
	if (nick.size() > 0)
		msg = "462 " + nick + " :You may not reregister\r\n";
	else
		msg = "462 :You may not reregister\r\n";
	return (msg);
}

std::string ServerMsg::NEEDMOREPARAMS(std::string nick, std::string cmd)
{
	std::string msg;
	if (nick.size() > 0)
		msg = "461 " + nick + " " + cmd + " :Not enough parameters\r\n";
	else
		msg = "461 " + cmd + " :Not enough parameters\r\n";
	return (msg);
}

std::string ServerMsg::PASSWDMISMATCH()
{
	std::string msg;
	msg = "464 :Password incorrect\r\n";
	return (msg);
}

std::string ServerMsg::NOTREGISTERD(std::string nick)
{
	std::string msg;
	if (nick.size() > 0)
		msg = "451 " + nick + " :You have not registerd\r\n";
	else
		msg = "451 :You have not registerd\r\n";
	return (std::string());
}

std::string ServerMsg::NONICKNAMEGIVEN(std::string nick)
{
	std::string msg;
	if (nick.size() > 0)
		msg = "431 " + nick + " :No nickname given\r\n";
	else
		msg = "431 :No nickname given\r\n";
	return (msg);
}

std::string ServerMsg::ERRONEUSNICKNAME(std::string nick, std::string errNick)
{
	std::string msg;
	if (nick.size() > 0)
		msg = "432 " + nick + " " + errNick + " :Erroneus nickname\r\n";
	else
		msg = "432 " + errNick + " :Erroneus nickname\r\n";
	return (msg);
}

std::string ServerMsg::NICKNAMEINUSE(std::string nick, std::string errNick)
{
	std::string msg;
	msg = "433 " + nick + " " + errNick + " :Nickname is already in use\r\n";
	return (msg);
}

std::string ServerMsg::NICKCHANGE(std::string oldNick, std::string hostname,
								  std::string servername, std::string newNick)
{
	std::string msg;
	msg = ":" + oldNick + "!" + hostname + "@" + servername + " NICK :" + newNick + "\r\n";
	return (msg);
}

std::string ServerMsg::PINGMSG(std::string serverIP)
{
	std::string msg;
	msg = "PING " + serverIP + " :" + serverIP + "\r\n";
	return (msg);
}

std::string ServerMsg::NOORIGIN(std::string nick)
{
	std::string msg;
	msg = "409 " + nick + " :No origin specified\r\n";
	return (msg);
}

std::string ServerMsg::NOSUCHSERVER(std::string nick, std::string server)
{
	std::string msg;
	msg = "402 " + nick + " " + server + ":No such server\r\n";
	return (msg);
}

std::string ServerMsg::WELCOME(std::string nick, std::string hostname,
							std::string servername)
{
	std::string msg;
	msg = ":" + Server::getIP() + " 001 " + nick + " :Welcome to the FT_IRC Network, "
		+ nick + "!" + hostname + "@" + servername + "\r\n";
	return (msg);
}

std::string ServerMsg::NOMOTD(std::string nick)
{
	std::string msg;
	msg = ":" + Server::getIP() + " 422 " + nick + " :MOTD File is missing\r\n"; 
	return (msg);
}

std::string ServerMsg::INVALIDCHANNEL(std::string nick, std::string server)
{
	std::string msg;
	msg = "476 " + nick + " " + server + " :Invalid channel name\r\n";
	return (msg);
}


std::string ServerMsg::TOPIC(std::string nick, std::string channel, 
						std::string topic)
{
	std::string msg;
	msg = "332 " + nick + " " + channel + " :" + topic + "\r\n";
	return (msg);
}

std::string ServerMsg::TOPICINFO(std::string nick, std::string channel,
							std::string topicWriter, int time)
{
	std::stringstream ss;
	ss << time;
	std::string msg;
	msg = "333 " + nick + " " + channel + " " + topicWriter + " :" + ss.str() + "\r\n";
	return (msg);
}

std::string ServerMsg::BADCHANNELKEY(std::string nick, std::string channel)
{
	std::string msg;
	msg = "475 " + nick + " " + channel + " :Cannot join channel (+k)\r\n";
	return (msg);	
}

std::string ServerMsg::INVITEONLYCHAN(std::string nick, std::string channel)
{
	std::string msg;
	msg = "473 " + nick + " " + channel + " :Cannot join channel (+i)\r\n";
	return (msg);
}

std::string ServerMsg::CHANNELISFULL(std::string nick, std::string channel)
{
	std::string msg;
	msg = "471 " + nick + " " + channel + " :Cannot join channel (+l)\r\n";
	return (msg);
}

std::string ServerMsg::JOIN(std::string nick, std::string hostname,
						std::string servername, std::string channel)
{
	std::string msg;
	msg = ":" + nick + "!" + hostname + "@" + servername + " JOIN " + channel + "\r\n";
	return (msg);
}

std::string ServerMsg::PING(std::string cmd1, std::string cmd2)
{
	std::string msg;
	if (cmd2.size() == 0)
		msg = "PONG " + cmd1 + "\r\n";
	else
		msg = "PONG " + cmd1 + " " + cmd2 + "\r\n";
	return (msg);
}

std::string ServerMsg::NOSUCHCHANNEL(std::string nick, std::string channel)
{
	std::string msg;
	msg = "403 " + nick + " " + channel + " :No such channel\r\n";
	return (msg);
}

std::string ServerMsg::NOTONCHANNEL(std::string nick, std::string channel)
{
	std::string msg;
	msg = "442 " + nick + " " + channel + " :You're not on that channel\r\n";
	return (msg);
}

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

std::string ServerMsg::QUITMSGTOCHANNEL(std::string nick, std::string hostname,
									std::string servername, std::string message)
{
	std::string msg;
	msg = ":" + nick + "!" + hostname + "@" + servername + " QUIT :Quit: " + message + "\r\n";
	return (msg);
}

std::string ServerMsg::QUITCHANNEL(std::string hostname, std::string servername, std::string message)
{
	std::string msg;
	msg = "ERROR :Closing link: (" + hostname + "@" + servername + ") [Quit: " + message + "]\r\n";
	return (msg);
}

std::string ServerMsg::NORECIPIENT(std::string nick, std::string command)
{
	std::string msg;
	msg = "411 " + nick + " :No recipient given (" + command + ")\r\n";
	return (msg);
}

std::string ServerMsg::NOTEXTTOSEND(std::string nick)
{
	std::string msg;
	msg = "412 " + nick + " :No text to send\r\n";
	return (msg);
}

std::string ServerMsg::NOSUCHNICK(std::string clientNick, std::string findNick)
{
	std::string msg;
	msg = "401 " + clientNick + " " + findNick + " :No such nick\r\n";
	return (msg);
}

std::string ServerMsg::PRIVMSG(std::string nick, std::string hostname, 
							   std::string servername,	std::string receiver,
							   std::string message)
{
	std::string msg;
	msg = ":" + nick + "!" + hostname + "@" + servername + " PRIVMSG " + receiver + " :" + message + "\r\n";
	return (msg);
}

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
	msg = msg.substr(0, msg.size() - 1);
	msg += "\r\n";
	return (msg);
}
std::string ServerMsg::ENDOFNAMES(std::string nick, std::string channel)
{
	std::string msg;
	msg = "366 " + nick + " " + channel + " :End of /NAMES list\r\n";
	return (msg);
}

std::string ServerMsg::USERNOTINCHANNEL(std::string nick, std::string targetNick, std::string channel)
{
	std::string msg;
	msg = "441 " + nick + " " + targetNick + " " + channel + " :They are not on that channel\r\n";
	return (msg);
}

std::string ServerMsg::CHANOPRIVSNEEDED(std::string nick, std::string channel)
{
	std::string msg;
	msg = "482 " + nick + " " + channel + " :You must be a channel operator\r\n";
	return (msg);
}

std::string ServerMsg::KICK(std::string nick, std::string hostname, std::string servername,
							std::string channel, std::string targetNick, std::string message)
{
	std::string msg;
	msg = ":" + nick + "!" + hostname + "@" + servername + " KICK " + channel + " " + targetNick + " :" + message + "\r\n";
	return (msg);
}

std::string ServerMsg::USERONCHANNEL(std::string nick, std::string user, std::string channel)
{
	std::string msg;
	msg = "443 " + nick + " " + user + " " + channel + " :is already on channel\r\n";
	return (msg);
}

std::string ServerMsg::INVITE(std::string nick, std::string targetNick, std::string channel)
{
	std::string msg;
	msg = "341 " + nick + " " + targetNick + " :" + channel + "\r\n";
	return (msg);
}

std::string ServerMsg::INVITETOTARGET(std::string nick, std::string hostname, std::string servername,
									std::string targetNick, std::string channel)
{
	std::string msg;
	msg = ":" + nick + "!" + hostname + "@" + servername + " INVITE " + targetNick + " :" + channel + "\r\n";
	return (msg); 
}

std::string ServerMsg::INVITENOTICE(std::string nick, std::string channel, std::string targetNick)
{
	std::string msg;
	msg = "NOTICE " + channel + " :*** " + nick + " invited " + targetNick + " into the channel\r\n";
	return (msg);
}

std::string ServerMsg::NOTOPIC(std::string nick, std::string channel)
{
	std::string msg;
	msg = "331 " + nick + " " + channel + " :No toopic is set\r\n";
	return (msg);
}

std::string ServerMsg::CHANOPRIVSNEEDEDTOPICVERSION(std::string nick, std::string channel)
{
	std::string msg;
	msg = "482 " + nick + " " + channel + " :You do not have access to change the topic on this channel\r\n";
	return (msg);
}

std::string ServerMsg::TOPICCHANGE(std::string nick, std::string hostname, std::string servername,
									std::string channel, std::string topic)
{
	std::string msg;
	msg = ":" + nick + "!" + hostname + "@" + servername + " TOPIC " + channel + " :" + topic + "\r\n";
	return (msg);
}

std::string ServerMsg::CHANOPRIVSNEEDEDMODEVERSION(std::string nick, std::string channel, char mode)
{
	std::string msg;
	msg = "482 " + nick + " " + channel + " :You must have channel op access or above to set channel mode " + mode + "\r\n";
	return (msg);
}

std::string ServerMsg::UNKNOWNMODE(std::string nick, char mode)
{
	std::string msg;
	msg = "472 " + nick + " " + mode + " :is an unknown mode character\r\n";
	return (msg);
}

std::string ServerMsg::CHANNELMODEIS(std::string nick, std::string channel, std::string modeInfo)
{
	std::string msg;
	msg = "324 " + nick + " " + channel + " " + modeInfo + "\r\n";
	return (msg);
}

std::string ServerMsg::CHANNELINFO(std::string nick, std::string channel, int channelCreatedTime)
{
	std::stringstream ss;
	ss << channelCreatedTime;
	std::string msg;
	msg = "329 " + nick + " " + channel + " :" + ss.str() + "\r\n";
	return (msg);
}

std::string ServerMsg::KEYMODENOPARAM(std::string nick, std::string channel)
{
	std::string msg;
	msg = "696 " + nick + " " + channel + " k * :You must specify a parameter for the key mode. Syntax: <key>.\r\n";
	return (msg);
}

std::string ServerMsg::LIMITMODENOPARAM(std::string nick, std::string channel)
{
	std::string msg;
	msg = "696 " + nick + " " + channel + " l * :You must specify a parameter for the limit mode. Syntax: <limit>.\r\n";
	return (msg);
}

std::string ServerMsg::OPMODENOPARAM(std::string nick, std::string channel)
{
	std::string msg;
	msg = "696 " + nick + " " + channel + " o * :You must specify a parameter for the op mode. Syntax: <nick>.\r\n";
	return (msg);
}

std::string ServerMsg::UNKNOWNMODEKEY(std::string nick)
{
	std::string msg;
	msg = "472 " + nick + " :Invalid key\r\n";
	return (msg);
}

std::string ServerMsg::UNKNOWNMODELIMIT(std::string nick)
{
	std::string msg;
	msg = "472 " + nick + " :Invalild limit\r\n";
	return (msg);
}

std::string ServerMsg::MODE(std::string nick, std::string hostname, std::string servername,
							std::string channel, std::string modeInfo)
{
	std::string msg;
	msg = ":" + nick + "!" + hostname + "@" + servername + " MODE " + channel + " " + modeInfo + "\r\n";
	return (msg);
}