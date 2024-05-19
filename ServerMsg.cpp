#include "ServerMsg.hpp"
#include "Server.hpp"

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
	msg = ":" + oldNick + "!" + hostname + "@" + servername + " NICK " + newNick + "\r\n";
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
	std::string msg;
	std::stringstream ss(time);
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
