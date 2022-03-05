SRCS		= srcs/pipex.c srcs/external_command.c
LIB			= pipelib/ft_strjoin.c pipelib/ft_strlen.c pipelib/ft_substr.c 
LIB			:= $(LIB) pipelib/ft_strchr_replace.c pipelib/ft_split.c
HEADER		= include/pipex.h
RM			= rm -rf
NAME		= pipex
CC			= gcc
OBJECTS		= ${SRCS:.c=.o} ${LIB:.c=.o}
CFLAGS		= -Wall -Werror -Wextra

${NAME}:	${HEAD} ${OBJECTS}
	${CC} ${CFLAGS} -o ${NAME} ${OBJECTS}

all:		${NAME}

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:		
	${RM} ${OBJECTS} ${OBJECTSB}

fclean:
	${RM} ${OBJECTS} ${OBJECTSB}
	${RM} ${NAME} ${NAMEB}

re:			fclean all

# ================================ #
# ========== MAKE BONUS ========== #
# ================================ #

NAMEB		= pipexbonus
SRCSB		= srcs/pipex_bonus.c srcs/external_command.c
OBJECTSB	= ${SRCSB:.c=.o} ${LIB:.c=.o}

${NAMEB}:	${OBJECTSB} 
			${CC} ${CFLAGS} -o ${NAMEB} ${OBJECTSB}

bonus:		${NAMEB}

reb:		fclean bonus

.PHONY:		all clean fclean re bonus reb