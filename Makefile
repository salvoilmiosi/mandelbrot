CXX = g++
LD = g++
CFLAGS = -g -Wall --std=c++11

LDFLAGS =
LIBS = -lSDL2

INCLUDE = include
BIN_DIR = bin
OBJ_DIR = obj

MAKE = make

OUT_BIN = minesweeper

resource_pack_dir = resource_pack
resource_pack = $(resource_pack_dir)/bin/resource_pack
resource_load = $(resource_pack_dir)/bin/libresource_load.a

ifeq ($(OS),Windows_NT)
	LDFLAGS += -mwindows -lmingw32 -lSDL2main
	OUT_BIN := $(OUT_BIN).exe
	resource_pack := $(resource_pack).exe
	MAKE := mingw32-make
endif

$(shell mkdir -p $(BIN_DIR) >/dev/null)
$(shell mkdir -p $(OBJ_DIR) >/dev/null)

DEPFLAGS = -MT $@ -MMD -MP -MF $(OBJ_DIR)/$*.Td

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(patsubst src/%,$(OBJ_DIR)/%.o,$(basename $(SOURCES))) $(resource_load)

all: $(BIN_DIR)/$(OUT_BIN)

clean:
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(resource_pack_dir) clean

$(BIN_DIR)/$(OUT_BIN): $(OBJECTS) $(BIN_DIR)/resource.dat
	$(LD) -o $(BIN_DIR)/$(OUT_BIN) $(OBJECTS) $(LDFLAGS) $(LIBS)

$(BIN_DIR)/resource.dat: resource/resource.txt $(resource_pack)
	resource_pack/bin/resource_pack $< $@

$(resource_load): $(resource_pack)
$(resource_pack):
	$(MAKE) -C $(resource_pack_dir)

$(OBJ_DIR)/%.o : src/%.cpp
$(OBJ_DIR)/%.o : src/%.cpp $(OBJ_DIR)/%.d
	$(CXX) $(DEPFLAGS) $(CFLAGS) -c -I $(INCLUDE) -o $@ $<
	@mv -f $(OBJ_DIR)/$*.Td $(OBJ_DIR)/$*.d

$(OBJ_DIR)/%.d: ;
.PRECIOUS: $(OBJ_DIR)/%.d

-include $(patsubst src/%,$(OBJ_DIR)/%.d,$(basename $(SOURCES)))
