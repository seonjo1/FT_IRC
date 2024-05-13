#ifndef EXECUTOR_HPP
# define EXECUTOR_HPP

# include "Client.hpp"

class Executor
{
public:
	Executor(char *password, std::vector<std::string> *nickList);
	~Executor();

	static void execute(std::string msg); // 명령어 실행하는 함수

private:
	std::string pass;
	std::vector<std::string> *nickList;
	std::map<std::string, Channel> channelList; // 채널 배열
	bool isValidNick(std::string& nick);
};

#endif
