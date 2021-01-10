##### GLOBALS #####

# Compiler-related
CC      := gcc
CC_ARGS := -std=c11 -Wall -Wextra -O2
CC_LINK := 

# Folder-related
SRC  := src
LIB  := $(SRC)/lib
BIN  := bin
OBJ  := $(BIN)/obj
INST := install
DATA := data
DIRS := $(BIN) $(OBJ) $(INST)

# Includes
INCL := -I$(LIB)



##### INTERACTIVE #####
ifdef DEBUG
cc_args := $(cc_args) -g
else
cc_args := $(cc_args) -DNDEBUG
endif



##### META RULES #####
.PHONY: default all install dirs clean server client
default: all

all: server client
clean:
	-find $(BIN) -type f ! -name "*.*" -delete
	-find $(OBJ) -type f -name "*.o" -delete
	-rm -rf $(INST)/*

dirs:
	mkdir -p $(DIRS)

install:
	./install_portlookup.sh



##### OBJECT RULES #####
$(OBJ)/%.o: $(LIB)/%.c | dirs
	$(CC) $(CC_ARGS) $(INCL) -o $@ -c $<
$(OBJ)/%.o: $(SRC)/%.c | dirs
	$(CC) $(CC_ARGS) $(INCL) -o $@ -c $<



##### EXECUTABLE RULES #####

# Rules to compile both portlookup executables and prepare them for install
$(BIN)/portlookupcli: $(OBJ)/PortLookupCLI.o | dirs
	$(CC) -o $@ $^ $(CC_LINK)
$(BIN)/portlookup: $(OBJ)/PortLookupDaemon.o | dirs
	$(CC) -o $@ $^ $(CC_LINK)
server: $(BIN)/portlookup $(BIN)/portlookupcli
	cp $(BIN)/portlookup $(INST)/portlookup
	cp $(BIN)/portlookupcli $(INST)/portlookupcli
