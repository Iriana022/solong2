NAME = so_long
CC = cc -Wall -Wextra -Werror
SRC = so_long.c
OBJ = so_long.o
libx = ./minilibx-linux

all: $(NAME)

$(NAME):
	$(CC) -g -o $(NAME) $(SRC) -I$(libx) -L$(libx) -lmlx -lX11 -lXext -lm

clean:
	rm -rf $(NAME)
