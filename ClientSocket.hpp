#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP

# include "ASocket.hpp"

class ClientSocket : public ASocket
{
public:
	ClientSocket();
	~ClientSocket();
	ClientSocket(const ClientSocket& copy);
	ClientSocket& operator=(const ClientSocket& copy);
	virtual void create();
}

#endif