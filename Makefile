CC = gcc
CPP = g++
LD = g++

NAME = vo

OBJ_DIR = obj
SRC_DIR = src

I_CMD = $(addprefix -I, $(SRC_DIR))
L_CMD = $(addprefix -L, )

FLAGS = $(I_CMD) $(L_CMD)
CC_FLAGS = -O3 -Wall $(FLAGS)
CPP_FLAGS = -O3 -Wall $(FLAGS)
LD_FLAGS = -lGL -lGLU -lSOIL -lglut $(FLAGS)

SRC = $(shell find $(SRC_DIR) -type f -regex ".*\.cpp")
OBJ = $(subst $(SRC_DIR), $(OBJ_DIR), $(addsuffix .o, $(basename $(SRC))))
DEP = $(OBJ:.o=.d)
EXC = $(NAME)

all: $(EXC)

$(EXC): $(OBJ)
	@echo [LD] $@
	@mkdir -p $(dir $@)
	@$(LD) $(OBJ) -o $@ $(LD_FLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo [CU] $<
	@mkdir -p $(dir $@)
	@$(CPP) $< -c -o $@ -MMD $(CPP_FLAGS)

clean:
	@echo [RM] cleaning
	@rm $(OBJ_DIR) $(INC_DIR) $(LIB_DIR) -rf
	
-include $(DEP)
