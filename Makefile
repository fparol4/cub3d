NAME = cub3D
ALIAS = cub3d

CC = cc
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -I. -Ilibrary/libft -Ilibrary/libft/lib/ft_gnline -Ilibrary/mlx
DEPFLAGS = -MMD -MP

# DIRECTORIES
DIR_SRC = source
DIR_OBJ = build
DIR_MLX = library/mlx
DIR_LIBFT = library/libft

# DEFINITIONS
SRC = $(shell find $(DIR_SRC) -type f -name '*.c')

OBJ = $(patsubst $(DIR_SRC)/%.c,$(DIR_OBJ)/%.o,$(SRC))
DEP = $(OBJ:.o=.d)
MLX = $(DIR_MLX)/libmlx.a
LIBFT = $(DIR_LIBFT)/libft.a
LIB = -L$(DIR_LIBFT) -lft -L$(DIR_MLX) -lmlx -lXext -lX11 -lm

all: $(NAME) $(ALIAS)

$(NAME): $(LIBFT) $(MLX) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $(NAME)

$(ALIAS): $(NAME)
	cp $(NAME) $(ALIAS)

$(LIBFT):
	@make -C $(DIR_LIBFT)

$(MLX):
	make -C $(DIR_MLX)

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

clean:
	@rm -rf $(DIR_OBJ)
	@make clean -C $(DIR_LIBFT)
	@make clean -C $(DIR_MLX)

fclean: clean
	@rm -f $(NAME) $(ALIAS)
	@make fclean -C $(DIR_LIBFT)

re: fclean all

bonus: all

-include $(DEP)

.PHONY: all clean fclean re bonus
