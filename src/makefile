CC = gcc
CFLAGS = -O2 -Wall
LIB_SDL = -lSDL2 -lSDL2_image

NAME = PrevIV

SRC = main.c libStdAux.c
HED = libStdAux.h

main: $(SRC) $(HED)
	$(CC) $(CFLAGS) $(LIB_SDL) $(SRC) -o $(NAME)

clean:
	rm -f $(NAME)
	@echo "Done cleaning!"