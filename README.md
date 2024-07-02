# FT_IRC
IRC 서버를 구현

## IRC 란?
IRC (Internet Relay Chat) 란 실시간 텍스트 기반 통신 프로토콜로, 사용자들이 채팅 채널에 접속하여 대화할 수 있도록 하는 프로그램이다.

## 사용법

### 서버 실행하는 법
```
./ircserv <port> <password>
```
- <port> : 서버 소켓을 binding 할 포트
- <password> : IRC 클라이언트가 PASS 인증할 때 사용할 password

### 클라이언트로 접속 하는 법

- irssi 라는 클라이언트 프로그램 사용 (다른 클라이언트 프로그램도 가능)
- irssi 를 실행하고 밑의 명령어 형식에 맞춰 irssi에 입력하면 서버와 연결 가능
  ```
  /connect -nocap <server_ip> <server_port> <password>
  ```
- 서버 실행시 출력되는 명령어는 localhost 기준으로 접속 가능한 명령어

### 클라이언트로 irc 사용하는 법
- 서버가 받는 IRC protocol 에 맞는 명령어와 클라이언트 프로그램에서 클라이언트가 입력하는 명령어의 형태가 같지 않음
- 클라이언트의 명령어를 클라이언트 프로그램이 IRC protocol 에 맞게 변환해서 보내줌
  
  - 개인 메시지 보내는 법
      ```
      <서버가 받는 명령어>
      PRIVMSG <receive nick> <contents>

      <irssi 에 클라이언트가 입력하는 명령어>
      /msg <receive nick> <contents
      ```
- 따라서 irssi 나 다른 클라이언트를 사용할 때, 필요한 명령어 형식을 GPT 에 물어보는 것을 추천

## 소켓 프로그래밍
- TCP/IP(v4) 프로토콜 사용
- I/O Multiplexing
  - kqueue 를 이용해 구현
  - 스레드와 프로세스 1 개만 사용
- 최적화를 위해 1 번의 event 발생시 소켓별로 출력을 버퍼에 모아놨다가 마지막에 한 번에 전송

## 참고한 IRC Protocol
- RFC 1495 IRC Protocol
  - https://datatracker.ietf.org/doc/html/rfc1459
  - 초기의 IRC protocol 로 대부분의 내용을 참고함
  - 서버 간의 통신은 구현 x

## irssi 자체 명령어

- **window go N**
  - N 번째 windwo로 이동하는 명령어
- **quote <cmd>**
  - irssi 관여 없이 cmd 를 서버에 그대로 전송
- **dcc send <nick> <path>**
  - path 에 있는 파일을 nick 에게 전송
- **dcc get <nick> <file>** 
  - nick 에게 받은 file 을 HOME 에 저장
- **exit**
  - irssi 종료

## 구현 명령어 (mandatory)

**인증 명령어**
irssi 에서 서버와 연결시 자동으로 보내주는 명령어

- **PASS**
  - 서버에 접속하기 위한 password 인증 명령어
    
- **NICK**
  - nickname 을 설정하는 명령어
  - irssi 가 자동으로 보내주는 명령어에서 hostname 을 nick 으로 설정
  - 인증 후 사용하면 nickname 변경 가능
  - irssi 명령어
  
    ```
    /nick <new_nickname>
    ```

- **USER**
  - user 의 기본 정보를 입력하는 명령어
    - 기본 정보
      - username
      - hostname
      - servername
      - realname

**기본 명령어**
- **JOIN**
  - 채널에 참가하는 명령어
  - irssi 명령어

    ```
    /join <channel>
    ```
  
- **PART**
  - 채널을 나가는 명령어
  - irssi 명령어

    ```
    /part <channel>
    ```
    
- **PRIVMSG**
  - 채널에 메시지 보내기
    - irssi 명령어
      - 원하는 채널 window 에서 그냥 메시지를 보내면 됨

  - 개인 메시지 보내기
    - irssi 명령어
    
      ```
      /msg <nick> <contents>
      ```

- **INVITE**
  - 채널에 유저 초대
  - irssi 명령어

    ```
    /invite <nick>
    ```
    
- **KICK**
  - 채널에서 유저 강퇴
  - irssi 명령어
    
    ```
    /kick <nick>
    ```
  
- **TOPIC**
  - 채널의 topic 확인
    - irssi 명령어

      ```
      /topic
      ```

  - 채널의 topic 변경
    - irssi 명령어

      ```
      /topic <contents>
      ```
      
- **QUIT**
  - 서버와의 연결 종료
  - irssi 명령어
    
    ```
    /quit {<msg>}
    ```
    - msg 는 선택사항
    
- **MODE**

  \+ 와 - 로 채널 옵션을 설정하는 명령어

  - **OPTION (i)**
    - +i : 채널을 초대로만 입장 가능하게 설정
    - -i : i 옵션 제거
    - irssi 명령어
  
      ```
      /mode +i
      /mode -i
      ```
      
  - **OPTION (l)**
    - +l <limit> : 채널 인원 제한을 <limit> 으로 설정
    - -l : l 옵션 제거
    - irssi 명령어
  
      ```
      /mode +l <limit>
      /mode -l
      ```
      
  - **OPTION (k)**
    - +k <key> : 채널 비밀번호를 <key> 로 설정
    - -k : k 옵션 제거
    - irssi 명령어
      
      ```
      /mode +k <key>
      /mode -k
      ```
      
  - **OPTION (t)**
    - +t : 채널에서 TOPIC 명령어를 operator 만 설정 가능
    - -t : t 옵션 제거
    - irssi 명령어
  
      ```
      /mode +t
      /mode -t
      ```
      
  
  - **OPTION (o)**
    - +o <nick> : <nick> 에게 operator 권한 부여
    - -o <nick> : <nick> 에게 operator 권한 박탈 
    - irssi 명령어
  
      ```
      /mode +o <nick>
      /mode -o <nick>
      ```
    
## 챗봇 (bonus)

**강화 챗봇**
- 채팅방마다 챗봇을 1 개씩 존재
- 챗봇의 이름 변경 가능
- 강화를 통해 챗봇의 레벨을 0 부터 10 까지 올릴 수 있음

**챗봇 명령어**

- **BOT NAME <channel> <name>**
  - channel 에 있는 챗봇의 이름을 name 으로 변경
  - irssi 명령어

    ```
    /quote BOT NAME <channel> <name>
    ```
    
- **BOT REINFORCE <channel>**
  - channel 에 있는 챗봇을 강화
  - 레벨이 올라갈 수록 성공 확률이 낮아짐
  - 강화 실패시 레벨 하락
  - Lv.0 ~ Lv.10 까지 존재
  - 1% 확률로 Lv.0 초기화
  - irssi 명령어

    ```
    /quote BOT REINFORCE <channel>
    ```

## build

```
make
```

- ircserv 실행파일 생성 (mandatory 기능)

```
make bonus
```

- ircserv 실행파일 생성 (bonus 기능 추가) 

```
make clean
```

- ircserv 만들때 생성된 .o 파일들 제거

```
make fclean
```

- make clean + ircserv 삭제

```
make re
```

- make fclean + make

---

<img width="204" alt="스크린샷 2024-07-02 오전 11 11 17" src="https://github.com/seonjo1/FT_IRC/assets/127729846/57368cca-d7bd-4d68-ad07-6d199d10b928">

