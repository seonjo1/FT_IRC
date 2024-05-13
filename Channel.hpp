#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Client.hpp"

class Channel
{
public:
	Channel();
	~Channel();

	void removeNick(std::string& nick);
private:
	std::vector<std::string> clientList;
	
};

#endif
