#include "Utils.hpp"

int	irc_atoi(char *str)
{
	int	i = 0;
	long long num = 0;

	if (!isdigit(*str))
		throw std::runtime_error("irc_atoi() error");
	while (isdigit(*str))
	{
		int digit = *str - '0';
		num = num * 10 + digit;
		str++;
		i++;
	}
	if (*str != '\0' || num > 65535 || i > 5)
		throw std::runtime_error("irc_atoi() error");
	return (num);
}