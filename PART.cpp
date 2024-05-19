#include "Executor.hpp"
#include "Server.hpp"

void Executor::parsePART(std::vector<std::string>& cmds, std::string& msg)
{

}

void Executor::PART(Client& client, std::vector<std::string>& cmds)
{

}

/*
127.000.000.001.53282-127.000.000.001.06667: PART #c

127.000.000.001.06667-127.000.000.001.53282: :nana_!codespace@127.0.0.1 PART :#c

127.000.000.001.06667-127.000.000.001.53272: :nana_!codespace@127.0.0.1 PART :#c



127.000.000.001.53282-127.000.000.001.06667: PART #c :a,b,c hi my

127.000.000.001.06667-127.000.000.001.53282: :nana_!codespace@127.0.0.1 PART #c :a,b,c hi my

127.000.000.001.06667-127.000.000.001.53272: :nana_!codespace@127.0.0.1 PART #c :a,b,c hi my

127.000.000.001.53272-127.000.000.001.06667: PING irc.example.com

127.000.000.001.06667-127.000.000.001.53272: :irc.example.com PONG irc.example.com :irc.example.com

127.000.000.001.53282-127.000.000.001.06667: PING irc.example.com

127.000.000.001.06667-127.000.000.001.53282: :irc.example.com PONG irc.example.com :irc.example.com

127.000.000.001.53282-127.000.000.001.06667: PART #a,#b,#c :hi my

127.000.000.001.06667-127.000.000.001.53282: :nana_!codespace@127.0.0.1 PART #a :hi my
:nana_!codespace@127.0.0.1 PART #b :hi my
:irc.example.com 442 nana_ #c :You're not on that channel
*/