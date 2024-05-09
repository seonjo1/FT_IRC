#ifndef SERVERSOCKET_HPP
# define SERVERSOCKET_HPP

# include "ASocket.hpp"

class ServerSocket : public ASocket
{
public:
	ServerSocket();
	~ServerSocket();
	ServerSocket(const ServerSocket& copy);
	ServerSocket& operator=(const ServerSocket& copy);
	virtual void create();
}

#endif