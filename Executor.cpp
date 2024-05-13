#include "Executor.hpp"

Executor::Executor(char *password, std::vector<std::string> *nickList)
	: pass(password), nickList(nickList) {};

Executor::~Executor() {};

void Executor::execute(std::string msg)
{
	
}

// nick이 유효한 nickname인지 확인
bool Executor::isValidNick(std::string& nick)
{
	int size = nick.size();
	if (size > 30) return (false); // 규칙 1. 길이 최대 30
	if (!isalpha(nick[0])) return (false); // 규칙 2. 맨 앞은 알파벳으로 시작
	
	std::string lowercase;
	for (int i = 1; i < size; i++)
	{
		if (nick[i] == ' ') return (false); // 규칙 3. 공백 포함 x
		else if (!isalpha(nick[i]) || !isdigit(nick[i]) // 규칙 4. 숫자, 알파벳, '-', '_' 만으로 구성
			|| nick[i] != '-' || nick[i] != '_')
			return (false);
		lowercase += tolower(nick[i]);
	}
	// 규칙 5. nickname 대소문자 상관없이 중복 금지
	if (find(nickList->begin(), nickList->end(), lowercase) != nickList->end()) return (false);
	
	// 유효한 닉네임이므로 nickList에 추가
	nickList->push_back(lowercase);
	return (true);
}

