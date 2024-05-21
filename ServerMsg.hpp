#ifndef SERVERMSG_HPP
# define SERVERMSG_HPP

# include <sstream>
# include <string>

class Channel;

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
	static std::string INVALIDCHANNEL(std::string nick, std::string server);

	static std::string TOPIC(std::string nick, std::string channel, 
							std::string topic);
	static std::string TOPICINFO(std::string nick, std::string channel,
								std::string topicWriter, int time);
	static std::string BADCHANNELKEY(std::string nick, std::string channel);
	static std::string INVITEONLYCHAN(std::string nick, std::string channel);
	static std::string CHANNELISFULL(std::string nick, std::string channel);
	static std::string JOIN(std::string nick, std::string hostname,
							std::string servername, std::string channel);
	static std::string PING(std::string cmd1, std::string cmd2);
	static std::string NOSUCHCHANNEL(std::string nick, std::string channel);
	static std::string NOTONCHANNEL(std::string nick, std::string channel);
	static std::string PART(std::string nick, std::string hostname, std::string servername,
							std::string channel, std::string message);
	static std::string QUITMSGTOCHANNEL(std::string nick, std::string hostname,
									std::string servername, std::string message);
	static std::string QUITCHANNEL(std::string hostname, std::string servername, std::string message);
	static std::string NORECIPIENT(std::string nick, std::string command);
	static std::string NOTEXTTOSEND(std::string nick);
	static std::string NOSUCHNICK(std::string clientNick, std::string findNick);
	static std::string PRIVMSG(std::string nick, std::string hostname, std::string servername,
								std::string receiver, std::string message);
	static std::string NAMREPLY(std::string nick, Channel& channel);
	static std::string ENDOFNAMES(std::string nick, std::string channel);
	static std::string USERNOTINCHANNEL(std::string nick, std::string targetNick, std::string channel);
	static std::string CHANOPRIVSNEEDED(std::string nick, std::string channel);
	static std::string KICK(std::string nick, std::string hostname, std::string servername,
							std::string channel, std::string targetNick, std::string message);
	static std::string USERONCHANNEL(std::string nick, std::string user, std::string channel);
	static std::string INVITE(std::string nick, std::string targetNick, std::string channel);
	static std::string INVITETOTARGET(std::string nick, std::string hostname, std::string servername,
								std::string targetNick, std::string channel);
	static std::string INVITENOTICE(std::string nick, std::string channel, std::string targetNick);
	static std::string NOTOPIC(std::string nick, std::string channel);
	static std::string CHANOPRIVSNEEDEDTOPICVERSION(std::string nick, std::string channel);
	static std::string TOPICCHANGE(std::string nick, std::string hostname, std::string servername,
									std::string channel, std::string topic);
	static std::string CHANOPRIVSNEEDEDMODEVERSION(std::string nick, std::string channel, char mode);
	static std::string UNKNOWNMODE(std::string nick, char mode);
	static std::string CHANNELMODEIS(std::string nick, std::string channel, std::string modeInfo);
	static std::string CHANNELINFO(std::string nick, std::string channel, int channelCreatedTime);

};

#endif
