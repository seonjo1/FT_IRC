#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <map>
# include <set>
# include <cctype>
# include <vector>
# include <unistd.h>
# include <algorithm>

# include "../utils/Message.hpp"
# include "../utils/ServerMsg.hpp"

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

	// 서버에서 소켓 메시지 받을때 사용하는 함수
	void receiveMsg(); // 클라이언트가 보낸 데이터를 받음
	bool isDisconnected(); // 클라이언트와 연결 여부 확인
	bool isCmdComplete(); // 버퍼에 있는 명령어 완성 여부 확인
	std::string getCmd(); // 버퍼에 있는 명령어 반환

	// 플래그 반환 함수
	bool isRegistered(); // 등록 여부 반환

	// nick 함수
	static bool isInvalidNick(std::string& nick); // nickname 유효성 검사
	static bool isNicknameInUse(std::string& nick); // nickname 사용중인지 검사
	void addNick(std::string nick); // nickname 추가
	void changeNick(std::string nick); // nickname 변경

	// 클라이언트에게 보낼 sendBuf 관리 함수
	void sendMsg(); // sendBuf에 있는 내용 클라이언트에게 전달하고 sendBuf 초기화
	void addToSendBuf(std::string msg); // sendBuf에 메시지 추가

	// channel 관련 함수
	void addJoinedChannels(Channel* channel); // 클라이언트가 참여한 채널에 추가
	void removeJoinedChannels(Channel* channel); // 클라이언트가 참여한 채널에서 제거
	bool isClientMemberOfChannel(std::string& channel); // 클라이언트가 해당 채널에 참여했는지 확인

	// getter
	static Client& getClient(std::string& nick); // nick과 일치하는 Client 반환
	int getFd(); // fd 전달
	bool getPassFlag(); // PassFlag 전달
	bool getNickFlag(); // NickFlag 전달
	bool getUserFlag(); // UserFlag 전달
	bool getQuitFlag(); // QuitFlag 전달
	bool getConnectFlag(); // ConnectFlag 전달
	std::string getHostName(); // hostname 전달
	std::string getServerName(); // servername 전달
	std::string& getNick(); // nickname 전달
	std::vector<Channel*>& getJoinedChannels(); // joinedChannels 전달

	// setter
	void setPassFlag(bool sign); // PassFlag 변경
	void setNickFlag(bool sign); // NickFlag 변경
	void setUserFlag(bool sign); // UserFlag 변경
	void setOrigin(bool sign); // Origin 변경
	void setQuitFlag(bool sign); // QuitFlag 변경
	void setConnectFlag(bool sign); // connectFlag 변경
	void setNick(std::string& newNick); // nickname 변경
	void setData(std::string& username, // data 값 변경
				 std::string& hostname,
				 std::string& servername,
				 std::string& realname);

	// 연산자 오버로딩
	bool operator==(const Client& rhs);
				 
private:
// 멤버 변수
	// 클라이언트로 부터 받는 메시지
	Message msg;
	
	// 클라이언트 관련 정보
	int fd; // 클라이언트와 연결된 소켓
	Data data; // 클라이언트 정보 모음
	std::string nickname; // nickname
	std::string sendBuf; // 전송 메시지 버퍼
	std::vector<Channel*> joinedChannels; // client가 가입한 channel 모음
	
	// flag 변수들
	bool origin; // 원본 클라이언트 여부
	bool passFlag; // PASS 명령어 완료 여부
	bool nickFlag; // NICK 명령어 완료 여부
	bool userFlag; // USER 명령어 완료 여부
	bool quitFlag; // 클라이언트 종료 여부
	bool connectFlag; // 클라이언트 종료 여부
	bool registerFlag; // register 여부

// 멤버 함수
	// nick 함수
	void removeNick(); // nickname 제거
};

#endif
