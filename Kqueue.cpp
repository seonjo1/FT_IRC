#include "Kqueue.hpp"

Kqueue::Kqueue()
{
	int fd = kqueue();
	if (fd == -1) throw std::runtime_error("kqueue() error");
}

Kqueue::Kqueue(const Kqueue& copy)
{
}

Kqueue::~Kqueue()
{
}

Kqueue& Kqueue::operator=(const Kqueue& copy)
{
}

void Kqueue::socketAdd(int socket)
{
	struct kevent tmp;
	EV_SET(&tmp, socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
	changeList.push_back(tmp);
}

int Kqueue::event()
{
	int eventNum = kevent(fd, &changeList[0], changeList.size(), eventList, EVENTLISTSIZE, 0);
	if (eventNum == -1) throw std::runtime_error("kevent() error");
	return (eventNum);
}

bool Kqueue::flagCheck(int idx)
{
	if (eventList[idx].flags == EV_ERROR)
		return (false);
	return (true);
}

int Kqueue::getFd(int idx)
{
	return (eventList[idx].ident);
}
