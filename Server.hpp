#ifndef SERVER_HPP
# define SERVER_HPP

# include "Socket.hpp"

class Server
{
public:
	Server(char *port, char *password);
	~Server();

	void run();

private:
	char *password;
};

#endif