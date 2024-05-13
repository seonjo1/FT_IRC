#ifndef ERRMSG_HPP
# define ERRMSG_HPP

# include <string>

class ErrMsg
{
public:
	static std::string ALREADYREGISTER(std::string& nick);
	static std::string NEEDMOREPARAMS(std::string& cmd);
	static std::string NEEDMOREPARAMS(std::string& nick, std::string& cmd);
	static std::string PASSWDMISMATCH();
	static std::string NOTREGISTERD();
	static std::string NONICKNAMEGIVEN();
	static std::string ERRONEUSNICKNAME(std::string& errNick);
	static std::string ERRONEUSNICKNAME(std::string& nick, std::string& errNick);
	static std::string NICKNAMEINUSE(std::string& errNick);
	static std::string NICKNAMEINUSE(std::string& nick, std::string& errNick);
};

#endif
