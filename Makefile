
NAME := "repos-dex.dylib"

default: all

all:
	clang -dynamiclib -std=gnu99 gnl.c idle.c message_reaload.c outils.c start.c wssplit.c -current_version 1.0 -compatibility_version 1.0 -fvisibility=hidden -o $(NAME)

clean:
	rm $(NAME)

fclean: clean
