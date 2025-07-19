NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror


RM = rm -f 

SRCS = philo.c \
	   monitor.c \
	   simulation.c \
	   philo_routine.c \
	   stop.c \
	   tools.c \


OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS )  -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
