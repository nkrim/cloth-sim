ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

CXX = g++
CXXFLAGS = -std=c++11 -Wall -g
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
OPENGLFLAGS = -framework OpenGL
NFDFLAGS = -framework AppKit

OUT = $(ROOT_DIR)/clothsim.out
OBJ_DIR = $(ROOT_DIR)/objs
SRC_DIR = $(ROOT_DIR)/src
RSC_DIR = $(ROOT_DIR)/resources
LIB_DIR = $(ROOT_DIR)/libs



# -- NFD CONFIG --
NFD_CONFIG=release_x64
NFD_DIR=$(ROOT_DIR)/libs/nfd/build/gmake_macosx
NFD_OBJ_DIR=$(NFD_DIR)/obj/x64/Release/nfd
NFD_SRC_DIR=$(ROOT_DIR)/libs/nfd/src
NFD_SRC_FILES=$(NFD_SRC_DIR)/nfd_common.c
NFD_OBJ_FILES=$(patsubst $(NFD_SRC_DIR)/%.c,$(NFD_OBJ_DIR)/%.o,$(NFD_SRC_FILES))

# -- SRC AND OBJ FILES FROM ./src --
# glob goes 2 levels deep
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp) \
			$(wildcard $(SRC_DIR)/*/*.cpp) \
			$(wildcard $(SRC_DIR)/*/*/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))



# -- MAIN PROGRAM COMPILATION -- 
all: $(NFD_OBJ_FILES) obj
	@$(eval OBJ_FILES=$(OBJ_FILES) $(wildcard $(NFD_OBJ_DIR)/*.o))
	@echo "-- Linking program..."
	@$(CXX) $(CXXFLAGS) $(SFMLFLAGS) $(OPENGLFLAGS) $(NFDFLAGS) -o $(OUT) $(OBJ_FILES)
	@echo "-- FINISHED -- program filename: clothsim.out\n"


# -- SRC FILE -> OBJ FILE --
.PHONY: obj
obj_echo:
	@echo "-- Compiling cloth-sim source files..."

obj: obj_echo $(OBJ_FILES)

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp
	@mkdir -p $(dir $@)
	@/bin/echo -n "$(patsubst $(SRC_DIR)/%,./src/%,$<) (no header): compiling..."
	@$(CXX) $(CXXFLAGS) -c -o $@ $<
	@echo "DONE"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	@mkdir -p $(dir $@)
	@/bin/echo -n "$(patsubst $(SRC_DIR)/%,./src/%,$<): compiling..."
	@$(CXX) $(CXXFLAGS) -c -o $@ $<
	@echo " DONE!"


# -- NFD MAKE --
.PHONY: nfd
$(NFD_OBJ_DIR)/nfd_common.o: $(NFD_SRC_DIR)/nfd_common.c
	@${MAKE} --no-print-directory -C $(NFD_DIR) -f $(NFD_DIR)/Makefile nfd
	@echo "---- nfd build complete ----\n"


# -- CLEAN --
.PHONY: clean cleanall
clean:
	rm -rf $(OBJ_DIR) $(NFD_DIR)/obj
cleanall: clean
	rm -f $(OUT)


# -- DEBUG --
print-%  : ; @echo $* = $($*)