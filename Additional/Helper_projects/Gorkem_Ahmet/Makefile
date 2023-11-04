# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gsever <gsever@student.42kocaeli.com.tr    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/18 18:46:27 by gsever            #+#    #+#              #
#    Updated: 2023/09/08 16:31:38 by gsever           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# https://www.freecodecamp.org/news/git-clone-branch-how-to-clone-a-specific-branch/

#!!!!!!!!! NOTE: < here > you need change with your customize. !!!!!!!
NAME		= ircserv
NAME_BOT	= ircbot

#	Flags for compile
CC			= c++
FLAGS		= -Wall -Werror -Wextra -std=c++98 -Wshadow -O3 \
				$(INCLUDE_FLAGS) $(LIBRARIES)

#	All process use for compiling.
UNAME		:= $(shell uname -s)
NUMPROC		:= 8

OS			:= NULL

# Checking if Brew is installed.
# IF_BREW_INSTALLED := $(shell which brew)
# Checking if Irssi is installed inside Brew.
# IF_IRSSI_INSTALLED := $(shell brew list --versions irssi)
# These 'brew --prefix' command returning brew's installed location.
BREW_PREFIX	:= $(shell brew --prefix)

OPENSSL_INC_OS	:= $(BREW_PREFIX)/Cellar/openssl/*/include
OPENSSL_LIB_OS	:= $(BREW_PREFIX)/Cellar/openssl/*/lib

INCLUDE_FLAGS	= \
	-I$(HEADERS_DIRECTORY) \
	-I$(HEADERS_DIRECTORY_BOT) \
	# -I$(OPENSSL_INC_OS)

#	Locations Part --> OK
LIBRARIES	= \
	# -L$(OPENSSL_LIB_OS)

# Getting all include and library files.
# BREW_INC_OS := $(shell find $(BREW_PREFIX)/Cellar -type d -name include)
# BREW_LIB_OS := $(shell find $(BREW_PREFIX)/Cellar -type d -name lib)
# Brew's all include and library files.
# BREW_INCLUDE_FLAGS := $(addprefix -I,$(BREW_INC_OS))
# BREW_LIBRARY_FLAGS := $(addprefix -L,$(BREW_LIB_OS))

HEADERS_DIRECTORY = includes
HEADERS		= $(wildcard $(HEADERS_DIRECTORY)/*.hpp)

SOURCES_DIRECTORY = sources
SOURCES		= $(wildcard $(SOURCES_DIRECTORY)/*.cpp) # All sources files.
COMMANDS_DIRECTORY = sources/commands
SOURCES		+= $(wildcard $(COMMANDS_DIRECTORY)/*.cpp) # All sources/commands files.

HEADERS_DIRECTORY_BOT = includes/bot
BOT_HEADERS		= $(wildcard $(HEADERS_DIRECTORY_BOT)/*.hpp)
BOT_DIRECTORY	= sources/bot
SOURCES_BOT			= $(wildcard $(BOT_DIRECTORY)/*.cpp) # All Bot's file.

OBJECTS_DIRECTORY = objects
OBJECTS		= $(addprefix $(OBJECTS_DIRECTORY)/, $(notdir $(SOURCES:%.cpp=%.o)))
OBJECTS_DIRECTORY_BOT = objects/bot
OBJECTS_BOT		+= $(addprefix $(OBJECTS_DIRECTORY_BOT)/, $(notdir $(SOURCES_BOT:%.cpp=%.o)))

# BOT_SRC		= $(foreach FILE, $(BOT), $(shell find $(SOURCES_DIR) -name $(FILE)))

vpath %.cpp $(SOURCES_DIRECTORY):$(COMMANDS_DIRECTORY) # Getting this locations's *.cpp files.
# vpath %.cpp $(BOT_DIRECTORY) # Getting this BOT location's *.cpp files.

#	COLORS --> 🟥 🟩 🟦
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

#	COLORS BOLD--> B🟥 B🟩 B🟦
B_CYAN		= \033[1;36m
B_BLUE		= \033[1;34m
B_YELLOW	= \033[1;33m
B_GREEN		= \033[1;32m
B_RED		= \033[1;31m
B_RESET		= \033[1m
#NOTE: \033[ ile derlenince calisiyor \e[ ile derlenince bozuk calisiyor.

#	Compiling with all threads.
ifeq ($(UNAME), Linux)
	NUMPROC	:= $(shell grep -c ^processor /proc/cpuinfo)
	OS = "You are connected from -$(CYAN)$(UNAME)$(X)- 🐧 Welcome -$(CYAN)$(USER)$(X)- 😄!"
else ifeq ($(UNAME), Darwin)
	NUMPROC	:= $(shell sysctl -n hw.ncpu)
	OS = "You are connected from 42 school's iMac 🖥 ! Welcome $(CYAN)$(USER)$(X)"
	ifeq ($(USER), yuandre)
		OS = "You are connected from -$(CYAN)MacBook$(X)- 💻 Welcome -$(CYAN)$(USER)$(X)-!"
	endif
endif
# You can use --> man sysctl -> shell: sysctl -a | grep "hw.ncpu"

all:
	@$(MAKE) $(NAME) -j $(NUMPROC) --no-print-directory
#	@$(MAKE) -s $(NAME) -j $(NUMPROC)

bonus:
	@$(MAKE) $(NAME_BOT) -j $(NUMPROC) --no-print-directory

#	Compiling
# $(OBJECTS_DIRECTORY)/%.o: $(SOURCES_DIRECTORY)/%.cpp | $(OBJECTS_DIRECTORY)
$(OBJECTS_DIRECTORY)/%.o: %.cpp | $(OBJECTS_DIRECTORY)
	@$(CC) $(FLAGS) -c $< -o $@
	@printf "%-57b %b" "$(BLUE)COMPILED $(CYAN)$@" "$(GREEN)[✓]$(X)\n"
# NOTE: $@ -> First $(<ARGUMAN>)
# NOTE: $< -> Second $(<ARGUMAN>)

$(NAME): $(OBJECTS_DIRECTORY) $(OBJECTS)
	@$(CC) $(FLAGS) $(OBJECTS) -o $(NAME)
	@printf "%-50b %b" "$(GREEN)CREATED $@" "[FINISHED]$(X)\n"
	@echo $(OS) Compiled with $(NUMPROC) cores!

# $(NAME_BOT): $(OBJECTS_DIRECTORY) $(BOT_DIRECTORY)/*.cpp
# @$(CC) $(FLAGS) $(BOT_DIRECTORY)/*.cpp -o $(NAME_BOT)
$(NAME_BOT): $(OBJECTS_DIRECTORY_BOT) $(OBJECTS_BOT)
	@$(CC) $(FLAGS) $(OBJECTS_BOT) -o $(NAME_BOT)
	@printf "%-50b %b" "$(GREEN)CREATED $@" "[FINISHED]$(X)\n"
	@echo $(OS) Compiled with $(NUMPROC) cores!

#	Objects file creating
$(OBJECTS_DIRECTORY):
	@if [ ! -d $@ ]; then \
		mkdir -p $@; \
		echo "$(NAME): $(GREEN)$@ folder was created$(RESET)"; \
	fi
# NOTE: $@ -> @(OBJECTS_DIRECTORY)
# @mkdir -p $(OBJECTS_DIRECTORY);
# @echo "$(NAME): $(GREEN)$(OBJECTS_DIRECTORY) was created$(RESET)";

#	Objects file creating
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

re:
	@$(MAKE) fclean --no-print-directory
	@$(MAKE) all --no-print-directory

print:
	@echo "$(BREW_PREFIX) burada."
	@echo "$(BREW_INC_OS) inc burada."
	@echo "$(BREW_LIB_OS) inc burada."
	@echo "$(FLAGS)"
	@grep -q '$(BREW_PREFIX)' ~/.zshrc && echo "Var" || echo "Yok"

open:
	@echo "$(BREW_PREFIX)"
	$(BREW_PREFIX)/bin/irssi

# Checking if brew(For install irssi) and irssi(IRC Client) is installed.
# install_brew:
# 	ifeq ($(IF_BREW_INSTALLED), )
# 		@echo "$(B_BLUE)Brew is not installed, installing 'Brew'...$(RESET)"
# 		@mkdir -p /goinfre/brew
# 		@git clone --depth=1 https://github.com/Homebrew/brew /goinfre/brew
# 		@if ! grep -q '$(BREW_PREFIX)/bin' $(HOME)/.zshrc; then \
# 			@echo 'export PATH=/goinfre/$(USER)/brew/bin:$(PATH)' >> $(HOME)/.zshrc; \
# 		@fi
# 		@echo "$(B_GREEN)Brew installed inside '$(BREW_PREFIX)'.$(RESET)"
# 	else
# 		@echo "$(B_YELLOW)Brew is already installed at '$(BREW_PREFIX)'.$(RESET)"
# 	endif
# 	ifeq ($(IF_IRSSI_INSTALLED), )
# 		@echo "$(B_BLUE)Brew is not installed, installing 'Irssi'.$(RESET)"
# 		brew install irssi
# 		@echo "$(B_GREEN)Irssi is installed inside '$(shell which irssi)'.$(RESET)"
# 	else
# 		@echo "$(B_YELLOW)Irssi is already installed at '$(shell which irssi)'.$(RESET)"
# 	endif

IF_BREW_INSTALLED := $(shell command -v brew 2> /dev/null)
IF_IRSSI_INSTALLED := $(shell command -v irssi 2> /dev/null)

install_brew:
	@if [ -z "$(IF_BREW_INSTALLED)" ]; then \
		echo "$(B_BLUE)Brew is not installed, installing 'Brew'...$(RESET)"; \
		mkdir -p $(BREW_PREFIX); \
		git clone --depth=1 https://github.com/Homebrew/brew $(BREW_PREFIX); \
		if ! grep -q '$(BREW_PREFIX)/bin' $(HOME)/.zshrc; then \
			echo 'export PATH=$(BREW_PREFIX)/bin:$$PATH' >> $(HOME)/.zshrc; \
		fi; \
		echo "$(B_GREEN)Brew installed inside '$(BREW_PREFIX)'.$(RESET)"; \
	else \
		echo "$(B_YELLOW)Brew is already installed at '$(BREW_PREFIX)'.$(RESET)"; \
	fi
	@if [ -z "$(IF_IRSSI_INSTALLED)" ]; then \
		echo "$(B_BLUE)Irssi is not installed, installing 'Irssi'.$(RESET)"; \
		brew install irssi; \
		echo "$(B_GREEN)Irssi is installed inside '$(shell which irssi)'.$(RESET)"; \
	else \
		echo "$(B_YELLOW)Irssi is already installed at '$(shell which irssi)'.$(RESET)"; \
	fi

valgrind:
	valgrind --leak-check=full ./$(NAME) 1234 abc

.PHONY: all clean fclean re bonus print open brew_readline