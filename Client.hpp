#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Message.hpp"
# include <map>
# include <cctype>
# include <vector>
# include <unistd.h>
# include <algorithm>

class Channel;

struct Data
{
	std::string username;
	std::string hostname;
	std::string servername;
	std::string realname;
};

class Client
{
public:
	Client(int fd);
	~Client();

	// 서버에서 사용하는 함수
	void receiveMsg(); // 클라이언트가 보낸 데이터를 받음
	bool isDisconnected(); // 클라이언트와 연결 여부 확인
	bool isCmdComplete(); // 버퍼에 있는 명령어 완성 여부 확인
	std::string getCmd(); // 버퍼에 있는 명령어 반환

	// 등록 확인 함수
	bool isRegistered();

	// nick 함수
	bool isInvalidNick(std::string& nick); // nickname 유효성 검사
	bool isNicknameInUse(std::string& nick); // nickname 사용중인지 검사
	void addNick(std::string& nick); // nickname 추가
	void removeNick(std::string& nick); // nickname 제거
	void changeNick(std::string& nick); // nickname 변경

	// 메시지 전송 함수
	void sendMsg(std::string msg);

	// channel 함수
	bool checkChannelParticipation(std::string& channel);

	// getter
	bool getPassFlag(); // PassFlag 전달
	bool getNickFlag(); // NickFlag 전달
	bool getUserFlag(); // UserFlag 전달
	std::string& getNick(); // nickname 전달
	int getFd(); // fd 전달

	// setter
	void setPassFlag(bool sign); // PassFlag 세팅
	void setNickFlag(bool sign); // NickFlag 세팅
	void setUserFlag(bool sign); // UserFlag 세팅
	void setNick(std::string& newNick); // nickname 세팅
	void setData(std::string& username, // data 값 세팅
				 std::string& hostname,
				 std::string& servername,
				 std::string& realname);
				 
private:
	Message msg; // 클라이언트와 통신할때 사용할 객체
	Data data; // 클라이언트 정보 모음
	std::string nickname; // nickname
	std::vector<Channel> joinedChannels; // client가 가입한 channel 모음
	int fd; // 클라이언트와 연결된 소켓
	bool passFlag; // PASS 명령어 완료 여부
	bool nickFlag; // NICK 명령어 완료 여부
	bool userFlag; // USER 명령어 완료 여부
};

#endif
