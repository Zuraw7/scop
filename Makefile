# Compiler
CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++11 -g3 -Iinclude

# Program name
NAME = scop

# Paths
SRC_PATH = src/
LIB_PATH = lib/
OBJ_PATH = obj/

# Libraries
LIBS = -lGL -lGLEW -lglfw

# Source files
SRC =  $(wildcard $(SRC_PATH)*.cpp) \
       $(wildcard $(SRC_PATH)core/*.cpp) \
       $(wildcard $(SRC_PATH)utils/*.cpp) \
       $(wildcard $(SRC_PATH)graphics/*.cpp) \
       $(wildcard $(SRC_PATH)render/*.cpp) \
       $(wildcard $(SRC_PATH)3rd/*.cpp) \
       $(wildcard $(SRC_PATH)textures/*.cpp)

LIB_SRC = $(wildcard $(LIB_PATH)**/*.cpp)

# Object files with full paths
OBJS = $(SRC:%.cpp=$(OBJ_PATH)%.o) $(LIB_SRC:%.cpp=$(OBJ_PATH)%.o)

# Generic compilation rule
$(OBJ_PATH)%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build program
$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS) $(LIBS)

# Main rules
all: $(NAME)

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
