CC = gcc
CFLAGS = -O2 -Wall
LIB_SDL = -lSDL2 -lSDL2_image
DEBUG_FLAGS = -DDEFAULT_DEBUG_STATUS=true 

NAME = PrevIV

SRC = main.c libStdAux.c libSdlInstance.c
HED = libStdAux.h libSdlInstance.h

SRC_DIR = src
BUILD_DIR = ../build

main: $(SRC) $(HED) $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LIB_SDL) $(SRC) -o $(BUILD_DIR)/$(NAME)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: main

memdebug: CFLAGS += $(DEBUG_FLAGS) -g
memdebug: main


clean:
	rm -f $(NAME)
	@echo "Done cleaning!"