#include "../../include/utils/Bot.hpp"
#include "../../include/core/Channel.hpp"

Bot::Bot() : name("Cardet"), level(0)
{
	title[0] = LV0;
	title[1] = LV1;
	title[2] = LV2;
	title[3] = LV3;
	title[4] = LV4;
	title[5] = LV5;
	title[6] = LV6;
	title[7] = LV7;
	title[8] = LV8;
	title[9] = LV9;
	title[10] = LV10;

	probablity[0] = 95;
	probablity[1] = 90;
	probablity[2] = 80;
	probablity[3] = 70;
	probablity[4] = 60;
	probablity[5] = 50;
	probablity[6] = 40;
	probablity[7] = 30;
	probablity[8] = 20;
	probablity[9] = 10;
};

Bot::~Bot() {};

void Bot::parseBOT(std::vector<std::string>& cmds, std::string& msg)
{
	std::stringstream ss(msg);
	std::string cmd;
	while (ss >> cmd)
		cmds.push_back(cmd);
}

void Bot::BOT(Client& client, std::vector<std::string>& cmds)
{
	// 인자 부족
	if (cmds.size() < 3)
	{
		client.sendMsg(ServerMsg::NEEDMOREPARAMS(client.getNick(), cmds[0]));
		return ;
	}

	// 없는 채널
	if (!Channel::isChannelInUse(cmds[2]))
	{
		client.sendMsg(ServerMsg::NOSUCHCHANNEL(client.getNick(), cmds[2]));
		return ;
	}

	// 내가 없는 채널
	Channel& channel = Channel::getChannel(cmds[2]);
	if (!channel.doesClientExist(client.getNick()))
	{
		client.sendMsg(ServerMsg::NOTONCHANNEL(client.getNick(), channel.getName()));
		return ;
	}

	if (cmds[1] == "REINFORCE")
	{
		//REINFORCE 실행
		Bot& bot = channel.getBot();
		bot.reinforceBot(channel);
	}
	else if (cmds[1] == "NAME")
	{
		// nick 인자 부족
		if (cmds.size() < 4)
		{
			client.sendMsg(ServerMsg::NEEDMOREPARAMS(client.getNick(), cmds[0]));
			return ;
		}
		
		// invalid nick
		if (Client::isInvalidNick(cmds[3]))
		{
			client.sendMsg(ServerMsg::ERRONEUSNICKNAME(client.getNick(), cmds[5]));
			return ;
		}
		
		// NAME 실행
		Bot& bot = channel.getBot();
		bot.changeName(cmds[3], channel);
	}
}

std::string Bot::getName()
{
	return (title[level] + name);
}

void Bot::changeName(std::string name, Channel& channel)
{
	channel.sendToClients(ServerMsg::BOTNICKCHANGE(this->name, title[level] + name));
	this->name = name;
	channel.sendToClients(ServerMsg::BOTPRIVMSG(title[level] + name, channel.getName(), "My name is " + title[level] + name));
}

void Bot::reinforceBot(Channel& channel)
{
	// 이미 만랩인 경우
	if (level == 10)
	{
		channel.sendToClients(ServerMsg::BOTPRIVMSG(title[level] + name, channel.getName(), "My level is already the maximum"));
		return ;
	}

	// 랜덤 수 얻기
	int randomNumber = rand() % 100;

	// 랜덤 값과 확률을 비교해 결과 반환
	if (randomNumber == 0)
	{
		if (level != 0)
			channel.sendToClients(ServerMsg::BOTNICKCHANGE(title[level] + name, title[0] + name));
		level = 0;
		channel.sendToClients(ServerMsg::BOTPRIVMSG(title[level] + name, channel.getName(), "1% fail GG!"));
	}
	else if (randomNumber <= probablity[level]) //success
	{
		channel.sendToClients(ServerMsg::BOTNICKCHANGE(title[level] + name, title[level + 1] + name));
		level++;
		channel.sendToClients(ServerMsg::BOTPRIVMSG(title[level] + name, channel.getName(), "Thank you for success in reinforcing me!"));
	}
	else //fail
	{
		if (level != 0)
		{
			channel.sendToClients(ServerMsg::BOTNICKCHANGE(title[level] + name, title[level - 1] + name));
			level--;
		}
		channel.sendToClients(ServerMsg::BOTPRIVMSG(title[level] + name, channel.getName(), "Reinforcement failed, please try one more time!"));
	}
}
