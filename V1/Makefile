NAME = cub3D

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

SR = $(wildcard $(DIR_SRC)/*.c)
OB = $(patsubst $(DIR_SRC)/%.c,$(DIR_OBJ)/%.o,$(SR))
DEP = $(OB:.o=.d)
MLX = $(DIR_MLX)/libmlx.a
LIBFT = $(DIR_LFT)/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(MLX) $(OB)
	$(CC) $(CF) $(OB) $(CL) $(CX) -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(DIR_LFT)

$(MLX):
	$(MAKE) -C $(DIR_MLX)

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CF) $(CI) -MMD -MP -c $< -o $@

clean:
	@rm -rf $(DIR_OBJ)
	@$(MAKE) clean -C $(DIR_LFT)
	@$(MAKE) clean -C $(DIR_MLX)

fclean: clean
	rm -f $(NAME)
	@$(MAKE) fclean -C $(DIR_LFT)

re: fclean all

-include $(DEP)

.PHONY: all clean fclean re
