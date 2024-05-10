#ifndef KQUEUE_HPP
# define KQUEUE_HPP

# include <vector>
# include <sys/event.h>

# define EVENTLISTSIZE 10

class Kqueue
{
public:
	Kqueue();
	Kqueue(const Kqueue& copy);
	~Kqueue();
	Kqueue&	operator=(const Kqueue& copy);

	int fd;
	std::vector<struct kevent> changeList;
	struct kevent eventList[EVENTLISTSIZE];

	void socketAdd(int socket);
	int event();
	bool flagCheck(int idx);
	int getFd(int idx);
};

#endif
