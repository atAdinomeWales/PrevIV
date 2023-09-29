CC = gcc
CFLAGS = -O2 -Wall
LIB_SDL = -lSDL2 -lSDL2_image
DEBUG_FLAGS = -DDEFAULT_DEBUG_STATUS=true 

NAME = PrevIV

SRC = main.c libStdAux.c libSdlInstance.c
HED = libStdAux.h libSdlInstance.h

main: $(SRC) $(HED)
	$(CC) $(CFLAGS) $(LIB_SDL) $(SRC) -o $(NAME)

debug: $(SRC) $(HED)
	$(CC) $(CFLAGS) $(LIB_SDL) $(DEBUG_FLAGS) $(SRC) -o $(NAME)

memdebug: $(SRC) $(HED)
	$(CC) $(CFLAGS) $(LIB_SDL) $(DEBUG_FLAGS) -g $(SRC) -o $(NAME)

clean:
	rm -f $(NAME)
	@echo "Done cleaning!"