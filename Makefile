NAME := "repos-dex.dylib"
SRC := gnl.c idle.c texte.c outils.c start.c wssplit.c

.SILENT: all
.PHONY: default all clean re

default: all

all:
	gcc -shared -o $(NAME) $(SRC)

clean:
	rm $(NAME)

re: clean
