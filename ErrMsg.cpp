#include "ErrMsg.hpp"

std::string ErrMsg::ALREADYREGISTER(std::string& nick)
{
	std::string msg;
	msg = "462 " + nick + " :You may not reregister\r\n";
	return (msg);
}

std::string ErrMsg::NEEDMOREPARAMS(std::string& cmd)
{
	std::string msg;
	msg = "461 " + cmd + " :Not enough parameters\r\n";
	return (msg);
}

std::string ErrMsg::NEEDMOREPARAMS(std::string& nick, std::string& cmd)
{
	std::string msg;
	msg = "461 " + nick + " " + cmd + " :Not enough parameters\r\n";
	return (msg);
}

std::string ErrMsg::PASSWDMISMATCH()
{
	return (std::string("464 :Password incorrect\r\n"));

}

std::string ErrMsg::NOTREGISTERD()
{
	return (std::string("451 :You have not registerd\r\n"));
}

std::string ErrMsg::NONICKNAMEGIVEN()
{
	return (std::string("431 :No nickname given\r\n"));
}

std::string ErrMsg::ERRONEUSNICKNAME(std::string& errNick)
{
	std::string msg;
	msg = "432 " + errNick + " :Erroneus nickname\r\n";
	return (msg);
}

std::string ErrMsg::ERRONEUSNICKNAME(std::string& nick, std::string& errNick)
{
	std::string msg;
	msg = "432 " + nick + " " + errNick + " :Erroneus nickname\r\n";
	return (msg);
}

std::string ErrMsg::NICKNAMEINUSE(std::string& errNick)
{
	std::string msg;
	msg = "433 " + errNick + " :Nickname is already in use\r\n";
	return (msg);
}

std::string ErrMsg::NICKNAMEINUSE(std::string& nick, std::string& errNick)
{
	std::string msg;
	msg = "433 " + nick + " " + errNick + " :Nickname is already in use\r\n";
	return (msg);
}
