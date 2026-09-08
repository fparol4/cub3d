NAME = cub3D

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iheader -Ilibrary/libft -Ilibrary/mlx

# DIRECTORIES
SRCS_DIR = source
OBJS_DIR = build
LIBFT_DIR = library/libft
MLX_DIR = library/mlx

# DEFINITIONS
SRCS = $(SRCS_DIR)/main.c
OBJS = $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)
LIBFT = $(LIBFT_DIR)/libft.a
MLX = $(MLX_DIR)/libmlx.a
LIBS = -L$(LIBFT_DIR) -lft -L$(MLX_DIR) -lmlx -lXext -lX11 -lm

all: $(NAME)

$(NAME): $(LIBFT) $(MLX) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(MLX):
	make -C $(MLX_DIR)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJS_DIR)
	@make clean -C $(LIBFT_DIR)
	@make clean -C $(MLX_DIR)

fclean: clean
	@rm -f $(NAME)
	@make fclean -C $(LIBFT_DIR)

re: fclean all

bonus: all

.PHONY: all clean fclean re bonus
