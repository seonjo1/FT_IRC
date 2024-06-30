#ifndef SOCKET_BONUS_HPP
# define SOCKET_BONUS_HPP

# include <sys/socket.h>
# include <arpa/inet.h>
# include <fcntl.h>

# include "Utils_bonus.hpp"

class Socket
{
public:
	static int servSocket; // 서버 소켓

	// 소켓 만드는데 사용되는 변수들
	static struct sockaddr_in addr; 
	static socklen_t addrLen;

	static int makeClientSocket(); // 클라이언트와 통신하는 소켓 만드는 함수
	static void makeServerSocket(char *port); // 서버 소켓 만드는 함수
};

#endif