CXX:= g++
NAME:= scop
CXXFLAGS:= -std=c++20 -ldl -lglfw -lGL
INCLUDES:= -I. -I inc -I glad/include
SRC_DIR:= src
SRCS:= main.cpp Window.cpp Shader.cpp Mesh.cpp
OBJ_DIR:= obj
OBJS = $(SRCS:%.cpp=$(SRC_DIR)/%.o)
GLAD:= glad/src/glad.c

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJS)
	$(CXX)  $(OBJS) $(GLAD) $(CXXFLAGS) $(INCLUDES) -o $(NAME)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	@rm -f $(OBJS) && echo "Cleaned *.o files"

fclean: clean
	@rm -f $(NAME) && echo "Cleaned $(NAME)"

re: fclean all

test: re
	./$(NAME)

.PHONY: all clean fclean re CXX NAME CXXFLAGS INCLUDES SRC_DIR SRCS OBJ_DIR OBJS GLAD 
