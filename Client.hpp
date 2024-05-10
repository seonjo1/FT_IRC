#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Message.hpp"

class Client
{
public:
	Client(int fd);
	~Client();

	void fillMsg();
	bool isDisconnected();
	bool isCmdComplete();
	std::string getMsg();

private:
	Message msg;
	int fd;
	int isRegist;
};

#endif
