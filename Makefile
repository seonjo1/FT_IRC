NAME		:=	ircserv
CPP			:=	c++
WFLAG		:=	-Wall -Wextra -Werror -std=c++98 -fsanitize=address

COREDIR			:=	./src/core/
COREBASE		:=	main Server Client Executor Channel

UTILSDIR		:= ./src/utils/
UTILSBASE		:=	Kqueue Message Utils Socket ServerMsg

CMDDIR			:= ./src/command/
CMDBASE			:= 	PASS NICK USER PONG \
					JOIN PING PART QUIT \
					PRIVMSG KICK INVITE \
					TOPIC MODE

SRC			:=	$(addprefix $(COREDIR), $(addsuffix .cpp, $(COREBASE))) \
				$(addprefix $(UTILSDIR), $(addsuffix .cpp, $(UTILSBASE))) \
				$(addprefix $(CMDDIR), $(addsuffix .cpp, $(CMDBASE)))

OBJ			:=	$(addprefix $(COREDIR), $(addsuffix .o, $(COREBASE))) \
				$(addprefix $(UTILSDIR), $(addsuffix .o, $(UTILSBASE))) \
				$(addprefix $(CMDDIR), $(addsuffix .o, $(CMDBASE)))


all : $(NAME)

$(NAME): $(OBJ)
	@$(CPP) $(WFLAG) $^ -o $@
	@echo "\033[32;1m\n🧊 [SUCCESS] $@ compiled successfully! ✨\033[m"

%.o : %.cpp
	@$(CPP) $(WFLAG) -I. -c $< -o $@
	@echo "\033[34;1m🎲 [OK] $@ is compiled\033[m"

clean :
	@rm -f $(OBJ)
	@echo "\033[35;1m🧹 [CLEAN] Object files have been removed!\033[m"

fclean : clean
	@rm -f $(NAME)
	@echo "\033[35;1m🧹 [FCLEAN] Executable files have been fully removed!\033[m"

re : fclean all

.PHONY : all clean fclean re
