NAME		:=	ircserv
CPP			:=	c++
WFLAG		:=	-Wall -Wextra -Werror -std=c++98 -fsanitize=address

DIR			:=	./
BASE		:=	main Server Socket Client Executor \
				Kqueue Message Utils Channel ServerMsg \
				PASS NICK USER PONG \
				JOIN PING PART QUIT \
				PRIVMSG KICK INVITE \
				TOPIC MODE
SRC			:=	$(addprefix $(DIR), $(addsuffix .cpp, $(BASE)))
OBJ			:=	$(addprefix $(DIR), $(addsuffix .o, $(BASE)))

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
