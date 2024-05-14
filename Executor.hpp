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
	std::string password; // PASS에 사용될 비밀번호
	std::vector<std::string> *nickList; // 닉네임 리스트
	std::map<int, Client> *clientList; // 클라이언트 리스트
	std::map<std::string, Channel> channelList; // 채널 리스트
	Kqueue *kq; // kqueue

	// nickname 
	bool isInvalidNick(std::string& nick); // nickname 유효성 검사
	bool isNicknameInUse(std::string& nick); // nickname 사용중인지 검사
	void addNick(std::string& nick); // nickname 추가
	void removeNick(std::string& nick); // nickname 제거

	std::vector<std::string> splitMsg(std::string &msg); // 명령어 공백 단위로 분리

	void sendMsg(int fd, std::string msg); // 클라이언트에게 메시지 보내기
	void PASS(Client& client, std::vector<std::string>& cmds);
	void NICK(Client& client, std::vector<std::string>& cmds);

};

#endif
