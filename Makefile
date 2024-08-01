NAME	=	philo
CFLAGS	=	-fsanitize=thread -g3 # -Wall -Wextra -Werror
FILES	=	philo.c \
			# utils.c \
			# threads.c \
			# monitor.c
OBJ		=	$(FILES:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	gcc $(CFLAGS) -o $(NAME) $(OBJ) -lpthread

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re