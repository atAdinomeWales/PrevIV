CC = gcc
CFLAGS = -O2 -Wall
LIB_SDL = -lSDL2 -lSDL2_image
DEBUG_FLAGS = -DDEFAULT_DEBUG_STATUS=true 

NAME = PrevIV

SRC = main.c libStdAux.c libSdlInstance.c
HED = libStdAux.h libSdlInstance.h

main: $(SRC) $(HED)
	$(CC) $(CFLAGS) $(LIB_SDL) $(SRC) -o $(NAME)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: main

memdebug: CFLAGS += $(DEBUG_FLAGS) -g
memdebug: main


clean:
	rm -f $(NAME)
	@echo "Done cleaning!"