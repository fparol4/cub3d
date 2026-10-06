NM = cub3D

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

SR = source/main.c
OB = $(SR:.c=.o)

all: $(NM)

%.o: %.c
	$(CC) $(CF) $(CI) -c $< -o $@

$(NM): $(OB)
	$(CC) $(OB) $(CL) $(CX) -o $(NM)

clean:
	@rm -f $(OB) $(OB:.o=.d)

fclean: clean
	rm -f $(NM)

re: fclean all

ex: all
	./$(NM)
	$(MAKE) fclean

.PHONY: all clean fclean re ex
