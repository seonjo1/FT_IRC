#include <iostream>
#include "Server.hpp"

int main(int argc, char **argv)
{
	if (argc != 3)
		std::cout << "Usage : " << argv[0] << " <port> <password>\n";
	else
	{
		try
		{
			Server server(argv[1], argv[2]);
			server.run();
			return (0);
		}
		catch(const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
	return (1);
}