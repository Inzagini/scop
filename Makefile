CXX:= g++
NAME:= scop
CXXFLAGS:= -std=c++20 -ldl -lglfw -lGL -g -fsanitize=address -MMD -MP
INCLUDES:= -I. -I inc -I glad/include
SRC_DIR:= src
SRCS:= main.cpp Window.cpp Shader.cpp Mesh.cpp Transform.cpp GameObject.cpp CameraControl.cpp parse.cpp 
OBJ_DIR:= obj
OBJS = $(SRCS:%.cpp=$(SRC_DIR)/%.o)
GLAD:= glad/src/glad.c
DEPS := $(OBJS:.o=.d)

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

test: $(NAME)
	LSAN_OPTIONS=suppressions=lsan.supp ./$(NAME)

-include $(DEPS)

.PHONY: all clean fclean re CXX NAME CXXFLAGS INCLUDES SRC_DIR SRCS OBJ_DIR OBJS GLAD 
