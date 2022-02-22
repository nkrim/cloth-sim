ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

CXX = g++
CXXFLAGS = -std=c++11 -Wall -g
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
OPENGLFLAGS = -framework OpenGL

OUT = $(ROOT_DIR)/clothsim.out
OBJ_DIR = $(ROOT_DIR)/objs
SRC_DIR = $(ROOT_DIR)/src
RSC_DIR = $(ROOT_DIR)/resources
LIB_DIR = $(ROOT_DIR)/libs

# glob goes 2 levels deep
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp) \
			$(wildcard $(SRC_DIR)/*/*.cpp) \
			$(wildcard $(SRC_DIR)/*/*/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

all: $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(SFMLFLAGS) $(OPENGLFLAGS) -o $(OUT) $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean cleanall
clean:
	rm -rf $(OBJ_DIR)
cleanall: clean
	rm -f $(OUT)

print-%  : ; @echo $* = $($*)