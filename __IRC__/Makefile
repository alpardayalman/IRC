NAME	= ircserv

#if on macosdo this
ifeq ($(shell uname), Darwin)
	CC		= c++ -g -std=c++98
else
	CC		= c++ -g
endif
CFLAGS	= -Wall -Wextra -Werror
INCLUDE	= -Iinclude/
SRC		= $(shell find source -name "*.cpp")
SRCDIR	= $(sort $(dir $(SRC))) 
SRCFLS	= $(notdir $(SRC))

OUTPRE	= builds/
OBJ		= $(addprefix $(OUTPRE), $(SRCFLS:.cpp=.o))

vpath %.cpp $(SRCDIR)
$(OUTPRE)%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

all: prefix $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OUTPRE)
	rm -rf $(NAME)

fclean: clean

re: fclean all

run: re
	@./ircserv 8080 123

prefix:
	mkdir -p $(OUTPRE)