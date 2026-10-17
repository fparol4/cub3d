CC      = cc
NAME    = libft.a
CFLAGS  = -Wall -Wextra -Werror -fPIC

SRC     = util/ft_isalpha.c \
          util/ft_isdigit.c \
          util/ft_isalnum.c \
          util/ft_isascii.c \
          util/ft_isprint.c \
          mem/ft_calloc.c \
          mem/ft_bzero.c \
          mem/ft_memset.c \
            mem/ft_memcpy.c \
            mem/ft_memmove.c \
            mem/ft_memchr.c \
            mem/ft_memcmp.c \
            mem/ft_secfree.c \
            num/ft_atoi.c \
          num/ft_itoa.c \
          num/ft_atof.c \
          num/ft_atoll.c \
          num/ft_digits.c \
           num/ft_signal.c \
          write/ft_putchar_fd.c \
           write/ft_putstr_fd.c \
           write/ft_putendl_fd.c \
           write/ft_putnbr_fd.c \
             lib/ft_printf/ft_printf.c \
             lib/ft_printf/ft_handler.c \
             lib/ft_gnline/get_next_line.c \
             lib/ft_gnline/get_next_line_utils.c \
            str/ft_toupper.c \
          str/ft_tolower.c \
          str/ft_strlen.c \
          str/ft_strlcpy.c \
          str/ft_strlcat.c \
          str/ft_strchr.c \
          str/ft_strrchr.c \
          str/ft_strncmp.c \
          str/ft_strnstr.c \
          str/ft_strdup.c \
          str/ft_substr.c \
          str/ft_strjoin.c \
          str/ft_strtrim.c \
          str/ft_split.c \
          str/ft_strmapi.c \
          str/ft_striteri.c \
          str/ft_streq.c \
          ds/l-list/ft_lstnew.c \
          ds/l-list/ft_lstadd_front.c \
          ds/l-list/ft_lstadd_back.c \
          ds/l-list/ft_lstlast.c \
          ds/l-list/ft_lstsize.c \
          ds/l-list/ft_lstdelone.c \
          ds/l-list/ft_lstclear.c \
          ds/l-list/ft_lstiter.c \
          ds/l-list/ft_lstmap.c \
          ds/d-list/ft_dlist.c \
          ds/d-list/ft_dlist_add.c \
          ds/d-list/ft_dlist_pop.c \
          ds/d-list/ft_dlist_mov.c \
          ds/d-array/ft_array.c \
          ds/d-array/ft_array_append.c

OBJS = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar -rcs $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
