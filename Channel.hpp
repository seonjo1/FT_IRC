#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Client.hpp"

class Channel
{
public:
    Channel();
    ~Channel();

private:
// 변수
    std::string channelName; // 채널 이름
    int timeChannelWasCreated// 채널 생성 시간

    std::vector<std::string> joinList// 참가자 명단
    std::vector<std::string> opList// operator 명단
    std::vector<std::string> inviteList// 초대명단

    std::string mode;// mode

    std::string topic;// topic
    std::string topicWriter// topic 작성자
    int timeTopicWasWritten// topic 생성시간
    
    std::string key;// 채널 key

    int limit;// limit

//함수
    // 채널 관리 (참가자 명단, operator 명단, 초대 명단)
        bool isInvalidChannel(std::string& channel); // channel 이름 유효성 검사
    	bool isChannelInUse(std::string& channel); // channel 이 있는지 검사
        void removeNickInChannel(std::string& nick);
        void addNickInChannel(std::string& nick);
        void changeNickInChannel(std::string& oldNick, std::string& newNick);

    // 채널에 메시지 전송
        void sendToClients(std::string& msg);
    // 채널에 클라이언트 있나 확인
        bool doesClientExist(std::string& nick);
    // 채널이 존재하는지 확인
        bool doesChannelExist(std::string& channel);
    // 채널 삭제 (채널리스트에서 삭제)
        void removeChannel(std::string& channel);
    // 채널 리스트에서 채널 찾아주는 함수
        Channel& findChannel(std::string& channel);
}

#endif