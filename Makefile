LIBFT_DIR = libft
SRC_DIR = src
OBJS_DIR = obj
LIBFT = $(LIBFT_DIR)/libft.a

SRCS = main expander change_line ft_echo ft_free redirection tokanizer get get2 get3 process find_path input_check error_handling here_doc redirection_checks
SRCS := $(addprefix $(SRC_DIR)/, $(SRCS))
SRCS := $(addsuffix .c, $(SRCS))
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJS_DIR)/%.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

NAME = minishell

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) inc/minishell.h
	$(CC) $(OBJS) -o $(NAME) -lreadline -L$(LIBFT_DIR) -lft

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c inc/minishell.h | $(OBJS_DIR)
			$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR):
			mkdir -p $(OBJS_DIR)

bonus: $(LIBFT) $(NAME)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJS_DIR)

fclean:
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -rf $(OBJS_DIR)
	$(RM) $(NAME)

re: fclean all

.PHONY: all bonus clean fclean re
