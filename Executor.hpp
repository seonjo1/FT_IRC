#ifndef EXECUTOR_HPP
# define EXECUTOR_HPP

# include <sstream>
# include "Client.hpp"
# include "ServerMsg.hpp"
# include "Kqueue.hpp"

# include <iostream>

class Executor
{
public:
	Executor(char *password);
	~Executor();

	void execute(Client& client, std::string msg); // 명령어 실행하는 함수

private:
	std::string password; // PASS에 사용될 비밀번호

	std::vector<std::string> parseMsg(std::string &msg); // 명령어 파싱

	void parsePASS(std::vector<std::string>& cmds, std::string& msg);
	void parseNICK(std::vector<std::string>& cmds, std::string& msg);
	void parseUSER(std::vector<std::string>& cmds, std::string& msg);
	void parsePONG(std::vector<std::string>& cmds, std::string& msg);
	void parseJOIN(std::vector<std::string>& cmds, std::string& msg);
	void parsePING(std::vector<std::string>& cmds, std::string& msg);
	void parsePART(std::vector<std::string>& cmds, std::string& msg);
	void parseQUIT(std::vector<std::string>& cmds, std::string& msg);
	void parsePRIVMSG(std::vector<std::string>& cmds, std::string& msg);
	void parseKICK(std::vector<std::string>& cmds, std::string& msg);
	void parseINVITE(std::vector<std::string>& cmds, std::string& msg);
	void parseTOPIC(std::vector<std::string>& cmds, std::string& msg);

	void PASS(Client& client, std::vector<std::string>& cmds);
	void NICK(Client& client, std::vector<std::string>& cmds);
	void USER(Client& client, std::vector<std::string>& cmds);
	void PONG(Client& client, std::vector<std::string>& cmds);
	void JOIN(Client& client, std::vector<std::string>& cmds);
	void PING(Client& client, std::vector<std::string>& cmds);
	void PART(Client& client, std::vector<std::string>& cmds);
	void QUIT(Client& client, std::vector<std::string>& cmds);
	void PRIVMSG(Client& client, std::vector<std::string>& cmds);
	void KICK(Client& client, std::vector<std::string>& cmds);
	void INVITE(Client& client, std::vector<std::string>& cmds);
	void TOPIC(Client& client, std::vector<std::string>& cmds);

};

#endif
