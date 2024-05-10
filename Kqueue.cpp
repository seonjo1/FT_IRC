#include "Kqueue.hpp"

Kqueue::Kqueue()
{
	fd = kqueue();
	if (fd == -1) throw std::runtime_error("kqueue() error");
}

Kqueue::~Kqueue() {};

void Kqueue::addSocket(int socket)
{
	struct kevent tmp;
	EV_SET(&tmp, socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
	changeList.push_back(tmp);
}

void Kqueue::removeSocket(int socket)
{
	std::vector<struct kevent>::iterator iter = changeList.begin();
	for (; iter != changeList.end(); iter++)
	{
		if (static_cast<int>(iter->ident) == socket)
			break;
	}
	changeList.erase(iter);
}

int Kqueue::event()
{
	int eventCnt = kevent(fd, &changeList[0], changeList.size(), eventList, EVENTLISTSIZE, 0);
	if (eventCnt == -1) throw std::runtime_error("kevent() error");
	return (eventCnt);
}

bool Kqueue::isEventTriggered(int idx)
{
	if (eventList[idx].flags == EV_ERROR)
		return (false);
	return (true);
}

int Kqueue::getFd(int idx)
{
	return (eventList[idx].ident);
}
