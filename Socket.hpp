#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <sys/socket.h>
# include <arpa/inet.h>
# include <stdexcept>
# include <fcntl.h>

class Socket
{
public:
	static int servSocket;
	static struct sockaddr_in addr;
	static socklen_t addrLen;

	static void makeServerSocket(char *port);
	static int makeClientSocket();
};

#endif