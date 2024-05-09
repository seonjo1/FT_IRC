#ifndef ASOCKET_HPP
# define ASOCKET_HPP

# include <sys/socket.h>

class ASocket
{
public:
	ASocket();
	~ASocket();
	ASocket(const ASocket& copy);
	ASocket& operator=(const ASocket& copy);
	virtual void create() = 0;

private:
	int _socket;
	struct sockaddr_in _addr;
	int _addrLen;
}

#endif