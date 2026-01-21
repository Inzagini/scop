CXX:=g++
CXXFLAGS:= -ldl -lglfw -std=c++20
INCS:=  -Iglad/include -Iinclude
GLAD:= glad/src/glad.c
SRCS_DIR:= src
FILES:= main.cpp
OBJ_DIR:= obj
OBJ:= $(patsubst $(SRCS_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
NAME:= scop


all: $(NAME) 

$(NAME): $(OBJ)
	$(CXX) $(OBJ) $(GLAD)  $(CXXFLAGS) $(INCS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCS) -c $< -o $@ 

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re CXX CXXFLAGS INCS GLAD FILES OBJ OBJ_DIR NAME
