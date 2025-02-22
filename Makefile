LIBFT_DIR = libft
SRC_DIR = src
OBJS_DIR = obj
LIBFT = $(LIBFT_DIR)/libft.a

SRCS = main expander change_line ft_echo ft_free redirection_make tokanizer \
		ft_export built_ins built_ins2 process find_path input_check here_doc redirection_checks \
		manage_fds process_utils here_doc_utils ft_general qauts_utils \
		general_utils change_shlvl ft_exit redirection_apply here_doc_set
SRCS := $(addprefix $(SRC_DIR)/, $(SRCS))
SRCS := $(addsuffix .c, $(SRCS))
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJS_DIR)/%.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror

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
