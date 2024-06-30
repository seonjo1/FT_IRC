NAME			:=	ircserv
CPP				:=	c++
WFLAG			:=	-Wall -Wextra -Werror -std=c++98

COREDIR			:=	./man/src/core/
COREBASE		:=	main Server Client Executor Channel

UTILSDIR		:=	./man/src/utils/
UTILSBASE		:=	Kqueue Message Utils Socket ServerMsg

CMDDIR			:=	./man/src/command/
CMDBASE			:= 	PASS NICK USER PONG \
					JOIN PING PART QUIT \
					PRIVMSG KICK INVITE \
					TOPIC MODE

SRC				:=	$(addprefix $(COREDIR), $(addsuffix .cpp, $(COREBASE))) \
					$(addprefix $(UTILSDIR), $(addsuffix .cpp, $(UTILSBASE))) \
					$(addprefix $(CMDDIR), $(addsuffix .cpp, $(CMDBASE)))

OBJ				:=	$(addprefix $(COREDIR), $(addsuffix .o, $(COREBASE))) \
					$(addprefix $(UTILSDIR), $(addsuffix .o, $(UTILSBASE))) \
					$(addprefix $(CMDDIR), $(addsuffix .o, $(CMDBASE)))


BON_NAME		:=	ircserv

BON_COREDIR		:=	./bon/src/core/
BON_COREBASE	:=	main Server Client Executor Channel

BON_UTILSDIR	:=	./bon/src/utils/
BON_UTILSBASE	:=	Kqueue Message Utils Socket ServerMsg Bot

BON_CMDDIR		:=	./bon/src/command/
BON_CMDBASE		:= 	PASS NICK USER PONG \
					JOIN PING PART QUIT \
					PRIVMSG KICK INVITE \
					TOPIC MODE

BON_SRC			:=	$(addprefix $(BON_COREDIR), $(addsuffix _bonus.cpp, $(BON_COREBASE))) \
					$(addprefix $(BON_UTILSDIR), $(addsuffix _bonus.cpp, $(BON_UTILSBASE))) \
					$(addprefix $(BON_CMDDIR), $(addsuffix _bonus.cpp, $(BON_CMDBASE)))

BON_OBJ			:=	$(addprefix $(BON_COREDIR), $(addsuffix _bonus.o, $(BON_COREBASE))) \
					$(addprefix $(BON_UTILSDIR), $(addsuffix _bonus.o, $(BON_UTILSBASE))) \
					$(addprefix $(BON_CMDDIR), $(addsuffix _bonus.o, $(BON_CMDBASE)))


all : $(NAME)

$(NAME): $(OBJ)
	@rm -rf $(BON_OBJ)
	@$(CPP) $(WFLAG) $^ -o $@
	@echo "\033[32;1m\n🧊 [SUCCESS] $@ compiled successfully! ✨\033[m"

bonus : $(BON_OBJ)
	@rm -rf $(OBJ)
	@$(CPP) $(WFLAG) $^ -o $(BON_NAME)
	@echo "\033[32;1m\n🧊 [SUCCESS] $@ compiled successfully! ✨\033[m"
	@touch bonus

%.o : %.cpp
	@$(CPP) $(WFLAG) -I. -c $< -o $@
	@echo "\033[34;1m🎲 [OK] $@ is compiled\033[m"

clean :
	@rm -f $(OBJ) $(BON_OBJ) bonus
	@echo "\033[35;1m🧹 [CLEAN] Object files have been removed!\033[m"

fclean : clean
	@rm -f $(NAME) $(BON_NAME)
	@echo "\033[35;1m🧹 [FCLEAN] Executable files have been fully removed!\033[m"

re : fclean all

.PHONY : all clean fclean re
