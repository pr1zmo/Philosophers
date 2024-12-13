NAME	=	philo
CFLAGS	=	-g3 -Wall -Wextra -Werror #-fsanitize=thread
FILES	=	main.c utils.c utils2.c actions.c \
			simulation.c init.c
OBJ_DIR	=	objects
OBJ		=	$(addprefix $(OBJ_DIR)/, $(FILES:.c=.o))

$(OBJ_DIR)/%.o: %.c
	cc $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	cc $(CFLAGS) -o $(NAME) $(OBJ) -lpthread

%.o: %.c
	cc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
