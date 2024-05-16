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
	bool isInvalidChannelName(std::string& channel); // channel 이름 유효성 검사
	bool isChannelInUse(std::string& channel); // channel 이 존재하는지
	void addNickInChannel(Client& client);
	void removeNickInChannel(Client& client);
	void changeNickInChannel(Client& client, std::string& newNick);

	// getter
	std::string getName();

private:
// 변수
	std::string channelName; // 채널 이름
	int timeChannelWasCreated;// 채널 생성 시간

	std::vector<Client> joinList;// 참가자 명단
	std::vector<Client> opList;// operator 명단
	std::vector<std::string> inviteList;// 초대명단

	std::string mode;// mode

	std::string topic;// topic
	std::string topicWriter;// topic 작성자
	int timeTopicWasWritten;// topic 생성시간
		
	std::string key;// 채널 key

	int limit;// limit

//함수
	// 채널에 메시지 전송
		void sendToClients(std::string& msg);
	// 채널에 클라이언트 있나 확인
		bool doesClientExist(std::string& nick);
	// 채널 삭제 (채널리스트에서 삭제)
		void removeChannel(std::string& channel);
	// 채널 리스트에서 채널 찾아주는 함수
		Channel& findChannel(std::string& channel);

	// 모드 셋팅
		void setMode(std::string& cmd);

};

#endif