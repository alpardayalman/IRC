# # Compiler and compiler flags
# CXX = g++
# CXXFLAGS = -std=c++98 -Wall -Werror -Wextra

# # Directories
# SRC_DIR = source
# CMD_DIR = source/commands
# OBJ_DIR = obj
# BIN_DIR = bin

# # Source files and object files
# SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
# OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

# # Executable name
# TARGET = $(BIN_DIR)/exec

# all: $(TARGET)

# run: $(TARGET)
# 	@./bin/exec 8080 123

# $(TARGET): $(OBJ_FILES) | $(BIN_DIR)
# 	$(CXX) $(CXXFLAGS) -o $@ $(OBJ_FILES)

# $(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# $(OBJ_DIR) $(BIN_DIR):
# 	mkdir -p $@

# clean:
# 	rm -rf $(OBJ_DIR) $(BIN_DIR)

# .PHONY: all clean


# CXX = g++
# CXXFLAGS = -std=c++98 -Wall -Werror -Wextra

# SRC_DIR := source  # Replace 'src' with your source directory
# CMD_DIR := $(SRC_DIR)/commands
# BUILD_DIR := build

# SRCS := $(shell find $(SRC_DIR) $(CMD_DIR) -type f -name *.cpp)
# OBJS := $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SRCS:.cpp=.o))
# OBJS := $(patsubst $(CMD_DIR)/%, $(BUILD_DIR)/%, $(OBJS:.cpp=.o))

# TARGET := my_program  # Replace 'my_program' with your desired executable name

# $(TARGET): $(OBJS)
#     $(CXX) $(CXXFLAGS) $^ -o $@

# $(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
#     $(CXX) $(CXXFLAGS) -c $< -o $@

# $(BUILD_DIR)/%.o: $(CMD_DIR)/%.cpp | $(BUILD_DIR)
#     $(CXX) $(CXXFLAGS) -c $< -o $@

# $(BUILD_DIR):
#     mkdir -p $(BUILD_DIR)

# clean:
#     rm -rf $(BUILD_DIR) $(TARGET)

# .PHONY: clean


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

fclean: clean
	rm $(NAME)

re: fclean all

prefix:
	mkdir -p $(OUTPRE)