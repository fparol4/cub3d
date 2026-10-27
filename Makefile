NAME = cub3d

CC = cc
CF = -Wall -Wextra -Werror
CI = -I$(DIR_INC) -I$(DIR_LFT) -I$(DIR_MLX)
CL = -L$(DIR_LFT) -L$(DIR_MLX)
CX = -lft -lmlx -lXext -lX11 -lm

DIR_SRC = source
DIR_OBJ = build
DIR_INC = header
DIR_MLX = library/mlx
DIR_LFT = library/libft

SR = \
	$(DIR_SRC)/common.c \
	$(DIR_SRC)/game.c \
	$(DIR_SRC)/hook.c \
	$(DIR_SRC)/main.c \
	$(DIR_SRC)/parser/parser.c \
	$(DIR_SRC)/parser/parser_cells.c \
	$(DIR_SRC)/parser/parser_color.c \
	$(DIR_SRC)/parser/parser_config.c \
	$(DIR_SRC)/parser/parser_connectivity.c \
	$(DIR_SRC)/parser/parser_grid.c \
	$(DIR_SRC)/parser/parser_reader.c \
	$(DIR_SRC)/parser/parser_validate.c \
	$(DIR_SRC)/player.c \
	$(DIR_SRC)/raycast.c \
	$(DIR_SRC)/render/render_draw.c \
	$(DIR_SRC)/render/render_fov.c \
	$(DIR_SRC)/render/render_view.c \
	$(DIR_SRC)/render/render_wall.c \
	$(DIR_SRC)/runtime.c \
	$(DIR_SRC)/screen.c \
	$(DIR_SRC)/setup.c \
	$(DIR_SRC)/texture.c

OB = $(SR:$(DIR_SRC)/%.c=$(DIR_OBJ)/%.o)
DEP = $(OB:.o=.d)

MLX = $(DIR_MLX)/libmlx.a
LIBFT = $(DIR_LFT)/libft.a

all: $(NAME)

$(NAME): $(OB) $(LIBFT) $(MLX)
	$(CC) $(CF) $(OB) $(CL) $(CX) -o $(NAME)

$(LIBFT):
	$(MAKE) -sC $(DIR_LFT)

$(MLX):
	$(MAKE) -sC $(DIR_MLX)

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CF) $(CI) -MMD -MP -c $< -o $@

-include $(DEP)

clean:
	@rm -rf $(DIR_OBJ)
	@$(MAKE) clean -C $(DIR_LFT)
	@$(MAKE) clean -C $(DIR_MLX)

fclean: clean
	rm -f $(NAME)
	@$(MAKE) fclean -C $(DIR_LFT)

re: fclean all

.PHONY: all clean fclean re
