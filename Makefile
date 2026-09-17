NAME = cub3D

CC = cc
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -I. -Ilibrary/libft -Ilibrary/mlx
DEPFLAGS = -MMD -MP

# DIRECTORIES
DIR_SRC = source
DIR_OBJ = build
DIR_MLX = library/mlx
DIR_LIBFT = library/libft

# DEFINITIONS
SRC = $(DIR_SRC)/main.c \
	$(DIR_SRC)/game/general.c \
	$(DIR_SRC)/game/set.c \
	$(DIR_SRC)/game/hook.c \
	$(DIR_SRC)/game/render.c

OBJ = $(SRC:$(DIR_SRC)/%.c=$(DIR_OBJ)/%.o)
DEP = $(OBJ:.o=.d)
MLX = $(DIR_MLX)/libmlx.a
LIBFT = $(DIR_LIBFT)/libft.a
LIB = -L$(DIR_LIBFT) -lft -L$(DIR_MLX) -lmlx -lXext -lX11 -lm

all: $(NAME)

$(NAME): $(LIBFT) $(MLX) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $(NAME)

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
	@rm -f $(NAME)
	@make fclean -C $(DIR_LIBFT)

re: fclean all

bonus: all

-include $(DEP)

.PHONY: all clean fclean re bonus
