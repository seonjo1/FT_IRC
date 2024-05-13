#include "Executor.hpp"

Executor::Executor() {};

Executor::~Executor() {};

void Executor::execute(std::string msg, char *password)
{
	// 일단 클라이언트가 보낸 메시지 그대로 출력
	(void)password;
	std::cout << "<excute msg!!>\n";
	std::cout << msg << "\n";
}

