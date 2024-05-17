#ifndef SERVERMSG_HPP
# define SERVERMSG_HPP

# include <string>

class ServerMsg
{
public:
	static std::string ALREADYREGISTER(std::string nick);
	static std::string NEEDMOREPARAMS(std::string nick, std::string cmd);
	static std::string PASSWDMISMATCH();
	static std::string NOTREGISTERD(std::string nick);
	static std::string NONICKNAMEGIVEN(std::string nick);
	static std::string ERRONEUSNICKNAME(std::string nick, std::string errNick);
	static std::string NICKNAMEINUSE(std::string nick, std::string errNick);
	static std::string NICKCHANGE(std::string oldNick, std::string hostname,
									std::string servername, std::string newNick);
	static std::string PINGMSG(std::string serverIP);
	static std::string NOORIGIN(std::string nick);
	static std::string NOSUCHSERVER(std::string nick, std::string server);
	static std::string WELCOME(std::string nick, std::string hostname,
								std::string servername);
	static std::string NOMOTD(std::string nick);
};

#endif
