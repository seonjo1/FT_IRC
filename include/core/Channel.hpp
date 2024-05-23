#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <ctime>

# include "Client.hpp"
# include "../utils/Utils.hpp"
# include "../utils/Bot.hpp"

class Channel
{
public:
	Channel(std::string& channelName);
	~Channel();

	// 채널 관리
	static void joinChannel(Client& client, std::string& channelName, std::string param); // 채널에 참가
	static bool isChannelInUse(std::string channel); // channel 이 존재하는지
	static bool isInvalidChannelName(std::string channel); // channel 이름 유효성 검사
	static void removeChannel(std::string channel);  // 채널 삭제
	static Channel& addChannel(std::string& channel); // 채널 추가
	void fillSet(std::set<Client*>& set); // 채널 참여자의 fd들을 set에 추가
	void addMsgToClientsSendBuf(std::string msg); // 채널에 메시지 전송
	void addMsgToClientsSendBuf(std::string msg, Client& client); // 채널에 메시지 전송 (client 제외)
	void addToInviteList(std::string nick); // 초대 리스트에 클라이언트 추가
	void removeFromInvitedList(std::string nick); // 초대 리스트에 클라이언트 삭제
	void addNickInChannel(Client& client); // channel에 nick 추가
	void removeNickInChannel(Client& client); // channel에 nick 삭제 (클라이언트가 갖고 있는 joinChannels는 따로 삭제해야되고 빈방이 될경우 채널 삭제도 따로 해야댐)
	void changeNickInChannel(Client& client, std::string& newNick, std::set<Client*>& set); // channel에 nick 변경
	bool doesClientExist(std::string& nick); // 채널에 클라이언트 존재하는지 확인

	// mode 함수
	static bool isInvalidKey(std::string key);
	static bool isInvalidLimit(std::string limit);
	bool doesTopicExist();
	bool isKeyMode();
	bool isLimitMode();
	bool isTopicMode();
	bool isInviteMode();
	bool isOperator(std::string nick);
	bool isInvitedClient(std::string nick);

	// getter
	static Channel& getChannel(std::string& channel); // 채널 찾아서 반환
	int getSize();
	int	getTopicTime();
	int getCreatedTime();
	bool getTopicFlag();
	Bot& getBot();
	std::string getKey();
	std::string getName();
	std::string getLimit();
	std::string getTopic();
	std::string getTopicWriter();
	std::string getModeInfo(std::string nick);
	std::vector<Client*>& getOpList();
	std::vector<Client*>& getJoinList();
	std::vector<std::string>& getInviteList();


	// setter
	void setTopic(std::string nick, std::string topic);
	void setInviteMode(bool sign);
	void setTopicMode(bool sign);
	void setKeyMode(bool sign, std::string key);
	void setLimitMode(bool sign, std::string limit);
	void setOPMode(bool sign, std::string nick);

private:
// 변수
	std::string channelName; // 채널 이름
	int timeChannelWasCreated;// 채널 생성 시간

	std::vector<Client*> joinList;// 참가자 명단
	std::vector<Client*> opList;// operator 명단
	std::vector<std::string> inviteList;// 초대명단
	Bot bot; // Chat Bot

	std::string mode;// mode
	bool topicFlag; // topic 존재 여부
	std::string topic;// topic
	std::string topicWriter;// topic 작성자
	int timeTopicWasWritten;// topic 생성시간
		
	std::string key;// 채널 key

	std::string limit;// limit

};

#endif