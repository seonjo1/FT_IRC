#ifndef BOT_BONUS_HPP
# define BOT_BONUS_HPP

# include <string>
# include <vector>
# include <sstream>
# include <random>

# define LV0 "[LV00]LIBFT-"
# define LV1 "[LV01]GNL-"
# define LV2 "[LV02]PUSH_SWAP-"
# define LV3 "[LV03]MINISHELL-"
# define LV4 "[LV04]CPP-"
# define LV5 "[LV05]INCEPTION-"
# define LV6 "[LV06]FT_TRANSCENDENCE-"
# define LV7 "[LV07]OuterCircle-"
# define LV8 "[LV08]Junior개발자-"
# define LV9 "[LV09]Senior개발자-"
# define LV10 "[LV10]연봉100억-한강뷰아파트보유-국가권력급개발자-"

class Client;
class Channel;

class Bot
{
public:
	Bot();
	~Bot();
	
	static void parseBOT(std::vector<std::string>& cmds, std::string& msg); // BOT 파싱
	static void BOT(Client& client, std::vector<std::string>& cmds); // BOT 명령 실행
	std::string getName(); // bot 이름 반환

private:
	// 멤버 변수
	std::string name; // bot 이름
	int level; // bot 레벨
	int probablity[10]; // 강화 확률
	std::string title[11]; // 레벨에 따른 타이틀

	// 멤버 변수
	void changeName(std::string name, Channel& channel); // bot 이름 변경
	void reinforceBot(Channel& channel); // bot 강화
};

#endif
