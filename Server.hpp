#ifndef SERVER_HPP
# include SERVER_HPP

class Server
{
private:

public:
	Server(char *port, char *password);
	~Server();

	void run();
}

#endif