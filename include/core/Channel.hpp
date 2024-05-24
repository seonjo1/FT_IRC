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
	static bool isChannelInUse(std::string channel); // channel 이 존재하는지 확인
	static bool isInvalidChannelName(std::string channel); // channel 이름 유효성 검사
	static void joinChannel(Client& client, std::string& channelName, std::string param); // 채널에 참가
	static Channel& addChannel(std::string& channel); // 채널 추가
	static void removeChannel(std::string channel);  // 채널 삭제
	
	// 채널 참가자 관리
	bool doesClientExist(std::string& nick); // 채널에 클라이언트 존재하는지 확인
	void addNickInChannel(Client& client); // channel에 nick 추가
	void removeNickInChannel(Client& client); // channel에 nick 삭제 (클라이언트가 갖고 있는 joinChannels는 따로 삭제해야되고 빈방이 될경우 채널 삭제도 따로 해야댐)
	void changeNickInChannel(Client& client, std::string& newNick, std::set<Client*>& set); // channel에 nick 변경
	void addToInviteList(std::string nick); // 초대 리스트에 클라이언트 추가
	
	// 채널 메시지 관련 함수
	void fillSet(std::set<Client*>& set); // 채널 참여자들을 set에 추가
	void addMsgToClientsSendBuf(std::string msg); // 채널에 메시지 전송
	void addMsgToClientsSendBuf(std::string msg, Client& client); // 채널에 메시지 전송 (client 제외)

	// 채널 mode 관련 함수
	static bool isInvalidKey(std::string key); // key가 올바른 형식인지 확인
	static bool isInvalidLimit(std::string limit); // limit이 올바른 형식인지 확인
	bool doesTopicExist(); // Topic이 존재하는지 확인
	bool isKeyMode(); // key mode인지 확인
	bool isLimitMode(); // limit mode인지 확인
	bool isTopicMode(); // topic mode인지 확인
	bool isInviteMode(); // invite mode인지 확인
	bool isOperator(std::string nick); // nick이 operator인지 확인
	bool isInvitedClient(std::string nick); // nick이 초대된 클라이언트인지 확인

	// getter
	static Channel& getChannel(std::string& channel); // 채널 찾아서 반환
	int getSize(); // 현재 채널 참여 인원 크기 반환
	int	getTopicTime(); // 토픽이 작성된 시간 반환
	int getCreatedTime(); // 채널이 생성된 시간 반환
	bool getTopicFlag(); // TopicFlag 반환
	Bot& getBot(); // Bot 반환
	std::string getKey(); // key 반환
	std::string getName(); // 채널 이름 반환
	std::string getLimit(); // limit 반환
	std::string getTopic(); // topic 반환
	std::string getTopicWriter(); // topic 작성자 반환
	std::string getModeInfo(std::string nick); // 채널 mode 정보 반환
	std::vector<Client*>& getOpList(); // opList 반환
	std::vector<Client*>& getJoinList(); // joinList 반환
	std::vector<std::string>& getInviteList(); // inviteList 반환

	// setter
	void setTopic(std::string nick, std::string topic); // topic, topic 작성자, topic 작성 시간 변경
	void setOPMode(bool sign, std::string nick); // op mode 변경
	void setKeyMode(bool sign, std::string key); // key mode 변경
	void setLimitMode(bool sign, std::string limit); // limit mode 변경
	void setTopicMode(bool sign); // topic mode 변경
	void setInviteMode(bool sign); // invite mode 변경

private:
// 멤버 변수
	// 채널 정보
	std::string channelName; // 채널 이름
	int timeChannelWasCreated;// 채널 생성 시간

	// 채널 참가자
	Bot bot; // Chat Bot
	std::vector<Client*> joinList;// 참가자 명단
	std::vector<Client*> opList;// operator 명단
	std::vector<std::string> inviteList;// 초대명단

	// 채널 mode
	std::string mode;// 적용중인 mode
	std::string key;// 채널 key
	std::string limit;// 채널 limit
	std::string topic;// 채널 topic
	std::string topicWriter;// topic 작성자
	bool topicFlag; // topic 존재 여부
	int timeTopicWasWritten;// topic 생성시간

// 멤버 함수
	// 채널 참가자 관리
	void removeFromInvitedList(std::string nick); // 초대 리스트에 클라이언트 삭제
};

#endif