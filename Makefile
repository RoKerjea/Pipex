SRCS	= main_pipex.c ft_strjoin.c ft_substr.c 
SRCS	:= $(SRCS) ft_strchr_replace.c ft_split.c external_command.c
HEAD	= pipex.h
RM		= rm -rf
NAME	= pipex
CC 		= gcc
OBJECTS	= ${SRCS:.c=.o}
PRINTER	= PrintError/libftprintf.a
CFLAGS	= -Wall -Werror -Wextra

${NAME}:	${HEAD} ${OBJECTS} 
	$(MAKE) -C PrintError
	${CC} ${CFLAGS} -o ${NAME} ${OBJECTS} ${PRINTER}

all:		${NAME}

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:		
	${RM} ${OBJECTS}
	$(MAKE) -C PrintError fclean

fclean:
	${RM} ${OBJECTS}
	$(MAKE) -C PrintError fclean
	${RM} ${NAME} 

re:			fclean all

.PHONY:		all clean fclean re