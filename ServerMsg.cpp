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
	if (nick.size() > 0)
		msg = "433 " + nick + " " + errNick + " :Nickname is already in use\r\n";
	else
		msg = "433 " + errNick + " :Nickname is already in use\r\n";
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
	msg = ":" + Server::IP + " 001 " + nick + " :Welcome to the FT_IRC Network, "
		+ nick + "!" + hostname + "@" + servername + "\r\n";
	return (msg);
}

std::string ServerMsg::NOMOTD(std::string nick)
{
	std::string msg;
	msg = ":" + Server::IP + " 422 " + nick + " :MOTD File is missing\r\n"; 
	return (msg);
}
