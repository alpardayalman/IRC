NAME		= ircserv
NAME_BOT	= ircbot

CC			= c++
FLAGS		= -Wall -Werror -Wextra -std=c++98 -Wshadow -O3 \
				$(INCLUDE_FLAGS) $(LIBRARIES)

UNAME		:= $(shell uname -s)
NUMPROC		:= 8

OS			:= NULL

INCLUDE_FLAGS	= \
	-I$(HEADERS_DIRECTORY) \
	-I$(HEADERS_DIRECTORY_BOT) \

LIBRARIES	= \


HEADERS_DIRECTORY = include
HEADERS		= $(wildcard $(HEADERS_DIRECTORY)/*.hpp)

SOURCES_DIRECTORY = source
SOURCES		= $(wildcard $(SOURCES_DIRECTORY)/*.cpp) # All sources files.
COMMANDS_DIRECTORY = source/commands
SOURCES		+= $(wildcard $(COMMANDS_DIRECTORY)/*.cpp) # All sources/commands files.

HEADERS_DIRECTORY_BOT = include/bot
BOT_HEADERS		= $(wildcard $(HEADERS_DIRECTORY_BOT)/*.hpp)
BOT_DIRECTORY	= source/bot
SOURCES_BOT			= $(wildcard $(BOT_DIRECTORY)/*.cpp) # All Bot's file.

OBJECTS_DIRECTORY = objects
OBJECTS		= $(addprefix $(OBJECTS_DIRECTORY)/, $(notdir $(SOURCES:%.cpp=%.o)))
OBJECTS_DIRECTORY_BOT = objects/bot
OBJECTS_BOT		+= $(addprefix $(OBJECTS_DIRECTORY_BOT)/, $(notdir $(SOURCES_BOT:%.cpp=%.o)))


vpath %.cpp $(SOURCES_DIRECTORY):$(COMMANDS_DIRECTORY) # Getting this locations's *.cpp files.
# vpath %.cpp $(BOT_DIRECTORY) # Getting this BOT location's *.cpp files.


BLACK	= \033[0;30m
RED		= \033[0;31m
GREEN	= \033[0;32m
YELLOW	= \033[0;33m
BLUE	= \033[0;34m
PURPLE	= \033[0;35m
CYAN	= \033[0;36m
WHITE	= \033[0;37m
END		= \033[m
RESET	= \033[0m
X		= \033[m


B_CYAN		= \033[1;36m
B_BLUE		= \033[1;34m
B_YELLOW	= \033[1;33m
B_GREEN		= \033[1;32m
B_RED		= \033[1;31m
B_RESET		= \033[1m

ifeq ($(UNAME), Linux)
	NUMPROC	:= $(shell grep -c ^processor /proc/cpuinfo)
else ifeq ($(UNAME), Darwin)
	NUMPROC	:= $(shell sysctl -n hw.ncpu)
endif


all:
	@$(MAKE) $(NAME) -j $(NUMPROC) --no-print-directory


bonus:
	@$(MAKE) $(NAME_BOT) -j $(NUMPROC) --no-print-directory


$(OBJECTS_DIRECTORY)/%.o: %.cpp | $(OBJECTS_DIRECTORY)
	@$(CC) $(FLAGS) -c $< -o $@
	@printf "%-57b %b" "$(BLUE)COMPILED $(CYAN)$@" "$(GREEN)[✓]$(X)\n"

$(NAME): $(OBJECTS_DIRECTORY) $(OBJECTS)
	@$(CC) $(FLAGS) $(OBJECTS) -o $(NAME)
	@printf "%-50b %b" "$(GREEN)CREATED $@" "[FINISHED]$(X)\n"
	@echo $(OS) Compiled with $(NUMPROC) cores!

$(NAME_BOT): $(OBJECTS_DIRECTORY_BOT) $(OBJECTS_BOT)
	@$(CC) $(FLAGS) $(OBJECTS_BOT) -o $(NAME_BOT)
	@printf "%-50b %b" "$(GREEN)CREATED $@" "[FINISHED]$(X)\n"
	@echo $(OS) Compiled with $(NUMPROC) cores!

$(OBJECTS_DIRECTORY):
	@if [ ! -d $@ ]; then \
		mkdir -p $@; \
		echo "$(NAME): $(GREEN)$@ folder was created$(RESET)"; \
	fi

$(OBJECTS_DIRECTORY_BOT):
	@if [ ! -d $@ ]; then \
		mkdir -p $@; \
		echo "$(NAME): $(GREEN)$@ folder was created$(RESET)"; \
	fi

clean: 
	@rm -rf $(OBJECTS_DIRECTORY)
	@echo "$(NAME): $(RED)$(OBJECTS) was deleted$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@rm -f $(NAME_BOT)
	@echo "$(NAME): $(RED)$(NAME) was deleted$(RESET)"
	@echo "$(NAME): $(RED)$(NAME_BOT) was deleted$(RESET)"

run: re
	@./ircserv 8080 123

re:
	@$(MAKE) fclean --no-print-directory
	@$(MAKE) all --no-print-directory

valgrind:
	valgrind --leak-check=full ./$(NAME) 8080 123

.PHONY: all clean fclean re bonus valgrind