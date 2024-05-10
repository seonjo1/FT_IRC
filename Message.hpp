#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <string>

class Message
{
private:

public:
	Message();
	Message(const Message& copy);
	~Message();
	Message&	operator=(const Message& copy);

	std::string buffer;
};

#endif
