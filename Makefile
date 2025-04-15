# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mzary <mzary@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/14 11:33:26 by mzary             #+#    #+#              #
#    Updated: 2025/04/15 11:11:20 by mzary            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		= cc
CFLAGS	= -Wall -Wextra -Werror

## minishell headers
MINISHELL_HDR	= inc/minishell.h
STRUCTS_HDR		= inc/structs.h
BUILTINS_HDR	= inc/builtins.h
EXECUTOR_HDR	= inc/executor.h
PARSER_HDR		= inc/parser.h
LEXER_HDR		= inc/lexer.h
TOOLS_HDR		= inc/tools.h

## src files
MINISHELL_FILES	= main.c
EXECUTOR_FILES	= executor.c getargs.c pipe.c redirection.c \
				  command.c hdoc.c redops.c
PARSER_FILES	= parse.c translate.c interpret.c \
				  expand.c separate.c asterisk.c \
				  analyse.c match.c remove.c
LEXER_FILES		= lexer.c lex.c spaceit.c splitit.c \
				  validate.c token.c
TOOLS_FILES		= strutils_0.c strutils_1.c strutils_2.c memutils.c \
				  envutils_0.c envutils_1.c envutils_2.c envutils_3.c \
				  cleanup.c reset.c error.c redutils.c treeutils.c \
				  futils.c siginit.c i32tostr.c status.c
BUILTINS_FILES	= cd.c echo.c env.c exit.c export.c d_export.c \
				  pwd.c unset.c builtins.c

## paths
MINISHELL_PATH	= src/
BUILTINS_PATH	= src/builtins/
EXECUTOR_PATH	= src/executor/
PARSER_PATH		= src/parser/
LEXER_PATH		= src/lexer/
TOOL_PATH		= src/tools/
OBJ_PATH		= obj/

## srcs
MINISHELL_SRCS	= $(addprefix $(MINISHELL_PATH), $(MINISHELL_FILES))
BUILTINS_SRCS	= $(addprefix $(BUILTINS_PATH), $(BUILTINS_FILES))
EXECUTOR_SRCS	= $(addprefix $(EXECUTOR_PATH), $(EXECUTOR_FILES))
PARSER_SRCS		= $(addprefix $(PARSER_PATH), $(PARSER_FILES))
LEXER_SRCS		= $(addprefix $(LEXER_PATH), $(LEXER_FILES))
TOOLS_SRCS		= $(addprefix $(TOOL_PATH), $(TOOLS_FILES))

## object files
MINISHELL_OBJS	= $(patsubst $(MINISHELL_PATH)%.c, $(OBJ_PATH)%.o, $(MINISHELL_SRCS))
BUILTINS_OBJS	= $(patsubst $(BUILTINS_PATH)%.c, $(OBJ_PATH)%.o, $(BUILTINS_SRCS))
EXECUTOR_OBJS	= $(patsubst $(EXECUTOR_PATH)%.c, $(OBJ_PATH)%.o, $(EXECUTOR_SRCS))
PARSER_OBJS		= $(patsubst $(PARSER_PATH)%.c, $(OBJ_PATH)%.o, $(PARSER_SRCS))
LEXER_OBJS		= $(patsubst $(LEXER_PATH)%.c, $(OBJ_PATH)%.o, $(LEXER_SRCS))
TOOLS_OBJS		= $(patsubst $(TOOL_PATH)%.c, $(OBJ_PATH)%.o, $(TOOLS_SRCS))

## final executable
NAME			= minishell

## all target
all: $(OBJ_PATH) $(NAME)
## create obj directory if it doesn't exist
$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)

## linking
$(NAME): $(MINISHELL_OBJS) $(EXECUTOR_OBJS) $(PARSER_OBJS) $(LEXER_OBJS) $(TOOLS_OBJS) $(BUILTINS_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lreadline

## compilation
$(OBJ_PATH)%.o: $(MINISHELL_PATH)%.c $(MINISHELL_HDR) $(STRUCTS_HDR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)%.o: $(EXECUTOR_PATH)/%.c $(EXECUTOR_HDR) $(STRUCTS_HDR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)%.o: $(PARSER_PATH)/%.c $(PARSER_HDR) $(STRUCTS_HDR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)%.o: $(LEXER_PATH)/%.c $(LEXER_HDR) $(STRUCTS_HDR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)%.o: $(BUILTINS_PATH)/%.c $(BUILTINS_HDR) $(STRUCTS_HDR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)%.o: $(TOOL_PATH)/%.c $(TOOLS_HDR) $(STRUCTS_HDR)
	$(CC) $(CFLAGS) -c $< -o $@

## clean target
clean:
	rm -fr $(OBJ_PATH)

## fclean target
fclean: clean
	rm -f $(NAME)

## re target
re: fclean all

## phony targets
.PHONY: all clean fclean re