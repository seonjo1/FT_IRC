#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <ctime>
# include "Client.hpp"

class Channel
{
public:
	Channel(std::string& channelName);
	~Channel();

	// 채널 관리 (참가자 명단, operator 명단, 초대 명단)
	static void removeChannel(std::string channel);  // 채널 삭제
	static void joinChannel(Client& client, std::string& channelName, std::string param); // 채널에 참가
	static bool isChannelInUse(std::string& channel); // channel 이 존재하는지
	static bool isInvalidChannelName(std::string& channel); // channel 이름 유효성 검사
	static Channel& addChannel(std::string& channel); // 채널 추가
	static Channel& findChannel(std::string& channel); // 채널 찾아서 반환
	void fillSetWithFd(std::set<int>& set); // 채널 참여자의 fd들을 set에 추가
	void sendToClients(std::string msg); // 채널에 메시지 전송
	void addNickInChannel(Client& client); // channel에 nick 추가
	void removeNickInChannel(Client& client); // channel에 nick 삭제
	void changeNickInChannel(Client& client, std::string& newNick, std::set<int>& set); // channel에 nick 변경
	bool doesClientExist(std::string& nick); // 채널에 클라이언트 존재하는지 확인

	// mode 함수
	bool isKeyMode();
	bool isInviteMode();
	bool isLimitMode();
	bool doesTopicExist();
	bool isInvitedClient(std::string nick);
	void removeClientFromInvitedList(std::string nick);

	// getter
	int getSize();
	int getLimit();
	int	getTopicTime();
	std::string getName();
	std::string getKey();
	std::string getTopic();
	std::string getTopicWriter();
	std::vector<Client*>& getJoinList();
	std::vector<Client*>& getOpList();
	std::vector<std::string>& getInviteList();


	// setter
	void setTopic(Client& client, std::string topic);

private:
// 변수
	std::string channelName; // 채널 이름
	int timeChannelWasCreated;// 채널 생성 시간

	std::vector<Client*> joinList;// 참가자 명단
	std::vector<Client*> opList;// operator 명단
	std::vector<std::string> inviteList;// 초대명단

	std::string mode;// mode

	bool topicFlag;
	std::string topic;// topic
	std::string topicWriter;// topic 작성자
	int timeTopicWasWritten;// topic 생성시간
		
	std::string key;// 채널 key

	int limit;// limit

// 함수
	void setMode(std::string& cmd);

};

#endif