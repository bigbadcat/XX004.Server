

TARGET = ServerGame
TARGET_PATH = ../../bin
FLAGS = -std=c++14
MACROS = -D NDEBUG

INCLUDE = ../Common ../ThirdPart/MySQL5.5/include
INCLUDE_PATH = $(addprefix -I,$(INCLUDE))
LIB_NAME = -lCommon -llua -ldl -lmysqlclient -lpthread -levent
LIB_PATH = -L../Common -L/usr/lib64/mysql
LIB_DEP = ../Common/libCommon.a

SRC = Game:Module:Module/Login:Module/Player
SRC_PATH = $(wildcard *.cpp)
SRC_PATH += $(foreach dir, $(subst :, ,$(SRC)), $(wildcard $(dir)/*.cpp))

OBJ_DIR = obj
OBJ_PATH = $(addprefix $(OBJ_DIR)/, $(subst :, , $(SRC)))
OBJS = $(addprefix $(OBJ_DIR)/,$(subst .cpp,.o,$(SRC_PATH)))
MAKE_OBJECT_DIR := $(shell mkdir -p $(OBJ_DIR) $(OBJ_PATH))

$(TARGET_PATH)/$(TARGET) : $(OBJS) $(LIB_DEP) 
	@echo "Linking... "
	g++ $(FLAGS) -o $@ $(OBJS) $(INCLUDE_PATH) $(LIB_NAME) $(LIB_PATH)
	@echo "build finished"

$(OBJS) : $(OBJ_DIR)/%.o : %.cpp $(OBJ_DIR)/%.cpp.d
	g++ $(FLAGS) -o $@ -c $< $(INCLUDE_PATH) $(MACROS)

DEPS = $(OBJS:.o=.cpp.d)
$(DEPS) : $(OBJ_DIR)/%.cpp.d : %.cpp
	g++ $(FLAGS) $< -MM $(INCLUDE_PATH) > $(OBJ_DIR)/$<.d

test:
	@echo $(SRC_PATH)
	@echo $(OBJ_PATH)
	@echo $(OBJS)
	@echo $(DEPS)


clean:
	@rm -rf ./obj
	@rm -rf $(TARGET_PATH)/$(TARGET)
	@echo "clean over"
