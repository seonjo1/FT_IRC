#ifndef KQUEUE_HPP
# define KQUEUE_HPP

# include <vector>
# include <sys/event.h>

class Kqueue
{
public:
	Kqueue();
	Kqueue(const Kqueue& copy);
	~Kqueue();
	Kqueue&	operator=(const Kqueue& copy);

	int kq;
	std::vector<struct kevent> changeList;
	struct kevent eventList[10];
};

#endif
