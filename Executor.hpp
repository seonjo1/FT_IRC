#ifndef EXECUTOR_HPP
# define EXECUTOR_HPP

# include <string>


# include <iostream>
class Executor
{
public:
	Executor();
	~Executor();

	static void execute(std::string msg, char *password); // 명령어 실행하는 함수
};

#endif
