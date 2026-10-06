NM = cub3D

CC = cc
CF = -Wall -Wextra -Werror -MMD -MP
CI = -I. -I$(DIR_LFT) -I$(DIR_MLX) -L$(DIR_LFT) -L$(DIR_MLX) -lft -lmlx -lXext -lX11 -lm

# DIR
DIR_SRC = source
DIR_OBJ = build
DIR_MLX = library/mlx
DIR_LFT = library/libft

# DEF

SR = source/main.c
OB = $(SR:.c=.o)

all: $(NM)

%.o: %.c
	$(CC) $(CF) -c $< -o $@

$(NM): $(OB)
	$(CC) $(CF) $(OB) $(CI) -o $(NM)

clean:
	@rm -f $(OB)

fclean: clean
	rm -f $(NM)

re: fclean all

.PHONY: all clean fclean re
