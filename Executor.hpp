#ifndef EXECUTOR_HPP
# define EXECUTOR_HPP

# include <sstream>
# include "Client.hpp"
# include "ErrMsg.hpp"
# include "Kqueue.hpp"

class Executor
{
public:
	Executor(char *password);
	~Executor();

	void execute(Client& client, std::string msg); // 명령어 실행하는 함수

private:
	std::string password; // PASS에 사용될 비밀번호

	std::vector<std::string> splitMsg(std::string &msg); // 명령어 공백 단위로 분리

	void PASS(Client& client, std::vector<std::string>& cmds);
	void NICK(Client& client, std::vector<std::string>& cmds);
	void USER(Client& client, std::vector<std::string>& cmds);

};

#endif
