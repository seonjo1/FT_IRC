#ifndef KQUEUE_HPP
# define KQUEUE_HPP

# include <vector>
# include <sys/event.h>

# define EVENTLISTSIZE 10

class Kqueue
{
public:
	Kqueue();
	~Kqueue();

	int fd;
	std::vector<struct kevent> changeList;
	struct kevent eventList[EVENTLISTSIZE];

	void addSocket(int socket);
	void removeSocket(int socket);
	int event();
	int getFd(int idx);
	bool isEventTriggered(int idx);
};

#endif
