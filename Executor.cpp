#include "Executor.hpp"

Executor::Executor() {};

Executor::~Executor() {};

void Executor::execute(std::string msg)
{
	std::cout << "<excute msg!!>\n";
	std::cout << msg << "\n";
}

