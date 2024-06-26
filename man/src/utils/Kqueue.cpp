#include "../../include/utils/Kqueue.hpp"

// 생성자
Kqueue::Kqueue()
{
	// 커널에 kqueue 를 만들고 할당된 fd 반환
	fd = kqueue();
	if (fd == -1) throw std::runtime_error("kqueue() error");
}

// 소멸자
Kqueue::~Kqueue() {};

// kqueue에 인자로 들어온 socket 추가
void Kqueue::addSocket(int socket)
{
	// kevent 구조체 세팅
	// ident = socket (fd)
	// filter = EVFILT_READ (소켓에서 읽을 수 있는 데이터가 있을때 이벤트 감지)
	// flags = EV_ADD (위 이벤트를 kqueue에 추가)
	//		 = EV_ENABLE (kqueue가 이벤트 발동했을때 반환하도록 한다)
	struct kevent tmp;
	EV_SET(&tmp, socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
	changeList.push_back(tmp);
}

// kqueue에서 인자로 들어온 socket 제거
void Kqueue::removeSocket(int socket)
{
	// changeList에 해당 socket을 제거
	std::vector<struct kevent>::iterator iter = changeList.begin();
	for (; iter != changeList.end(); iter++)
	{
		if (static_cast<int>(iter->ident) == socket)
			break;
	}
	changeList.erase(iter);
}

// kevent로 발생한 이벤트 개수 확인 후 반환
int Kqueue::event()
{
	int eventCnt = kevent(fd, &changeList[0], changeList.size(), eventList, EVENTLISTSIZE, 0);
	if (eventCnt == -1) throw std::runtime_error("kevent() error");
	return (eventCnt);
}


// 현재 발생한 이벤트에 오류가 발생했는지 확인
bool Kqueue::isNotEventError(int idx)
{
	if (eventList[idx].flags == EV_ERROR)
		return (false);
	return (true);
}

// 현재 발생한 이벤트의 fd 반환
int Kqueue::getFd(int idx)
{
	return (eventList[idx].ident);
}
