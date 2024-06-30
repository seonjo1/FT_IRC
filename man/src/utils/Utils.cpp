#include "../../include/utils/Utils.hpp"

// 0 ~ 65535(TCP/IP port 최댓값)의 범위만 반환하는 atoi
int	socket_atoi(char *str)
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

// 0 ~ 1000 까지 범위만 반환하는 atoi (방 최대인원 제한은 1000명까지 가능)
int	irc_atoi(std::string str)
{
	int	i = 0;
	long long num = 0;

	if (!isdigit(str[i]))
		return (-1);
	while (isdigit(str[i]))
	{
		int digit = str[i] - '0';
		num = num * 10 + digit;
		i++;
	}
	if (str[i] != '\0' || num > 1000 || i > 4)
		return (-1);
	return (num);
}
