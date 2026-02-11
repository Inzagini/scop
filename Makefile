CXX:= g++
NAME:= scop
CXXFLAGS:= -std=c++20 -ldl -lglfw -lGL -g -MMD -MP -Wall -Werror -Wextra
INCLUDES:= -I. -I inc -I glad/include
SRC_DIR:= src
SRCS:= main.cpp Window.cpp Shader.cpp Mesh.cpp Transform.cpp GameObject.cpp \
	CameraControl.cpp parse.cpp processInput.cpp MathUtils.cpp
OBJ_DIR:= obj
OBJS = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
GLAD:= glad/src/glad.c
DEPS := $(OBJS:.o=.d)
TEST_FILE := resources/teapot2.obj

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJS)
	$(CXX)  $(OBJS) $(GLAD) $(CXXFLAGS) $(INCLUDES) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	@rm -f $(OBJS) $(DEPS) && echo "Cleaned *.o files"

fclean: clean
	@rm -f $(NAME) && echo "Cleaned $(NAME)"

re: fclean all

test: $(NAME)
	./$(NAME) $(TEST_FILE)

supp: $(NAME)
	LSAN_OPTIONS=suppressions=lsan.supp ./$(NAME) $(TEST_FILE)

-include $(DEPS)

.PHONY: all clean fclean re CXX NAME CXXFLAGS INCLUDES SRC_DIR SRCS OBJ_DIR OBJS GLAD supp
