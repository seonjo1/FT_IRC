#ifndef BOT_HPP
# define BOT_HPP

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
	
	void changeName(std::string name, Channel& channel);
	void reinforceBot(Channel& channel);
	std::string getName();

	static void parseBOT(std::vector<std::string>& cmds, std::string& msg);
	static void BOT(Client& client, std::vector<std::string>& cmds);

private:
	std::string name;
	int level;
	int probablity[10];
	std::string title[11];
};

#endif
