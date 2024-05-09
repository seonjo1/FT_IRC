#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <sys/socket.h>
# include <arpa/inet.h>
# include <exception>

class Socket
{
public:
	static int servSocket;
	static struct sockaddr_in addr;
	static int addrLen;

	static void Init();
	static void makeServerSocket();
	static int makeClientSocket();
}

#endif