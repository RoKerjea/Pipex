SRCS	= main_pipex.c ft_strjoin.c ft_strlen.c ft_substr.c 
SRCS	:= $(SRCS) ft_strchr_replace.c ft_split.c external_command.c
HEAD	= pipex.h
RM		= rm -rf
NAME	= pipex
CC 		= gcc
OBJECTS	= ${SRCS:.c=.o}
CFLAGS	= -Wall -Werror -Wextra

${NAME}:	${HEAD} ${OBJECTS} 
	${CC} ${CFLAGS} -o ${NAME} ${OBJECTS}

all:		${NAME}

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:		
	${RM} ${OBJECTS}

fclean:		clean 
	${RM} ${NAME} 

re:			fclean all

.PHONY:		all clean fclean re