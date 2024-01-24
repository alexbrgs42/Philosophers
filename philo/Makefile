SRC =	sources/freeing.c \
		sources/initialization.c \
		sources/init_mutex.c \
		sources/utils.c \
		sources/eating.c \
		sources/finish.c \
		sources/count_meals.c \
		sources/main.c

OBJ = ${SRC:.c=.o}

CC = cc

FLAGS = -Wall -Werror -Wextra -pthread -g3

NAME = philo

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
