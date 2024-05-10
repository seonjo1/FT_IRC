#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <cerrno>
# include <string>
# include <sys/socket.h>

class Message
{
public:
	Message(int fd);
	~Message();

	bool isError();
	bool isDisconnected();
	void fillMsg();
	bool isComplete();
	std::string getMsg();

private:
	int fd;
	std::string buf;
	bool endFlag;
	bool errFlag;
};

#endif
