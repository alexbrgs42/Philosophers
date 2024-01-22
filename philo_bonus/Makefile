SRC =	sources/main.c \
		sources/freeing.c \
		sources/initialization.c \
		sources/start.c \
		sources/utils.c

OBJ = ${SRC:.c=.o}

CC = cc

FLAGS = -Wall -Werror -Wextra -g3

NAME = philo_bonus

.c.o:
	$(CC) $(FLAGS) -c $< -o $(<:.c=.o)

$(NAME):	$(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

all:	$(NAME)

clean:
	rm -rf $(OBJ)

fclean:	clean
	rm -rf $(NAME)

re:	fclean $(NAME)

.PHONY: all clean fclean re
