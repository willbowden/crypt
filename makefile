default: ./src/main.c
	clang -Wall -Werror -pedantic -ansi -fsanitize=address ./src/main.c -o ./bin/crypt