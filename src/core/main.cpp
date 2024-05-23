#include "../../include/core/Server.hpp"
#include <iostream>

int main(int argc, char **argv)
{
	if (argc != 3)
		std::cout << "Usage : " << argv[0] << " <port> <password>\n";
	else
	{
		try
		{
			std::cout << "<copy>\n/connect -nocap localhost " << argv[1] << " " << argv[2] << "\n";
			srand(static_cast<unsigned int>(time(0)));
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