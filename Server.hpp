#ifndef SERVER_HPP
# define SERVER_HPP

# include <map>
# include "Socket.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "Kqueue.hpp"

class Server
{
public:
	Server(char *port, char *password);
	~Server();


	Kqueue kq;
	std::map<int, Client> clientList;
	std::map<std::string, Channel> channelList;

	void run();
	void receiveClientRequest(int fd);

private:
	char *password;
};

#endif