#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Message.hpp"

class Client
{
public:
	Client();
	~Client();

private:
	Message msg;
	int isRegist;
};

#endif
