#ifndef KQUEUE_HPP
# define KQUEUE_HPP

# include <vector>
# include <sys/event.h>

# define EVENTLISTSIZE 10 // 발생한 이벤트를 저장할 수 있는 최대 크기

class Kqueue
{
public:
	Kqueue();
	~Kqueue();

	void addSocket(int socket); // kqueue에 이벤트 추가
	void removeSocket(int socket); // kqueue에 이벤트 삭제
	int event(); // kevent 함수로 발생한 이벤트 수 반환
	int getFd(int idx); // 발생한 이벤트의 fd 반환
	bool isNotEventError(int idx); // 발생한 이벤트에 에러 확인

private:
	int fd; // kqueue가 할당된 fd
	std::vector<struct kevent> changeList;  // kqueue에 등록한 이벤트들 모음
	struct kevent eventList[EVENTLISTSIZE]; // 발생한 이벤트를 저장할 배열
};

#endif
