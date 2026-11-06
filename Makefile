NAME = cub3d

CC = cc
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -I$(DIR_INC) -I$(DIR_LFT) -I$(DIR_MLX)
LDFLAGS = -L$(DIR_LFT) -L$(DIR_MLX)
LDLIBS = -lft -lmlx -lXext -lX11 -lm

DIR_SRC = source
DIR_OBJ = build
DIR_INC = header
DIR_MLX = library/mlx
DIR_LFT = library/libft

SOURCES = \
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
	$(DIR_SRC)/parser/parser_map_lines.c \
	$(DIR_SRC)/parser/parser_reader.c \
	$(DIR_SRC)/parser/parser_validate.c \
	$(DIR_SRC)/player.c \
	$(DIR_SRC)/raycast.c \
	$(DIR_SRC)/raycast_camera.c \
	$(DIR_SRC)/render/render_draw.c \
	$(DIR_SRC)/render/render_fov.c \
	$(DIR_SRC)/render/render_view.c \
	$(DIR_SRC)/render/render_wall.c \
	$(DIR_SRC)/runtime.c \
	$(DIR_SRC)/screen.c \
	$(DIR_SRC)/setup.c \
	$(DIR_SRC)/texture.c

OBJECTS = $(SOURCES:$(DIR_SRC)/%.c=$(DIR_OBJ)/%.o)
DEPS = $(OBJECTS:.o=.d)

MLX = $(DIR_MLX)/libmlx.a
LIBFT = $(DIR_LFT)/libft.a

all: $(NAME)

$(NAME): $(OBJECTS) $(LIBFT) $(MLX)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) $(LDLIBS) -o $(NAME)

$(LIBFT):
	$(MAKE) -sC $(DIR_LFT)

$(MLX):
	$(MAKE) -sC $(DIR_MLX)

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

clean:
	@rm -rf $(DIR_OBJ)
	@$(MAKE) clean -C $(DIR_LFT)
	@$(MAKE) clean -C $(DIR_MLX)

fclean: clean
	rm -f $(NAME)
	@$(MAKE) fclean -C $(DIR_LFT)

re: fclean all

.PHONY: all clean fclean re
