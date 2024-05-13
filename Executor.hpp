#ifndef EXECUTOR_HPP
# define EXECUTOR_HPP

# include <sstream>
# include "Client.hpp"
# include "ErrMsg.hpp"
# include "Kqueue.hpp"

class Executor
{
public:
	Executor(char *password, std::vector<std::string> *nickList,
		std::map<int, Client> *clinetList, Kqueue *kq);
	~Executor();

	void execute(Client& client, std::string msg); // 명령어 실행하는 함수

private:
	std::string password;
	std::vector<std::string> *nickList;
	std::map<int, Client> *clientList;
	std::map<std::string, Channel> channelList; // 채널 배열
	Kqueue *kq;

	bool isInvalidNick(std::string& nick);
	bool isNicknameInUse(std::string& nick);
	void addNick(std::string& nick);
	void removeNick(std::string& nick);

	std::vector<std::string> splitMsg(std::string &msg);

	void sendMsg(int fd, std::string msg);
	void PASS(Client& client, std::vector<std::string>& cmds);
	void NICK(Client& client, std::vector<std::string>& cmds);

};

#endif
