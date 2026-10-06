/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:20 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:20 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <limits.h>
# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_list
{
	void				*content;
	struct s_list		*next;
}						t_list;

typedef enum e_bool
{
	FALSE,
	TRUE
}						t_bool;

typedef struct s_dlist_node
{
	void				*content;
	int					index;
	struct s_dlist_node	*next;
	struct s_dlist_node	*previous;
}						t_dlist_node;

typedef struct s_dlist
{
	int					id;
	t_dlist_node		*head;
	t_dlist_node		*tail;
	size_t				size;
}						t_dlist;

typedef struct s_array
{
	void				*items;
	size_t				length;
	size_t				capacity;
	size_t				elem_size;
}						t_array;

/** util **/
int						ft_isalpha(int c);
int						ft_isdigit(int c);
int						ft_isalnum(int c);
int						ft_isascii(int c);
int						ft_isprint(int c);

/** mem **/
int						ft_memcmp(const void *s1, const void *s2, size_t n);
void					*ft_memset(void *b, int c, size_t len);
void					ft_bzero(void *b, size_t n);
void					*ft_memcpy(void *dst, const void *src, size_t n);
void					*ft_memmove(void *dst, const void *src, size_t n);
void					*ft_memchr(const void *s, int c, size_t n);
void					*ft_calloc(size_t count, size_t size);
void					*ft_secfree(char **buffer[], int size);

/** num **/
int						ft_atoi(const char *str);
int						ft_vatoi(const char *str);
long long				ft_atoll(const char *str);
int						ft_signal(int v);
int						ft_abs(int v);
size_t					ft_digits(size_t v, int size);
char					*ft_itoa(int n);
char					*ft_itoab(int v, char *base);
char					*ft_itoab_uns(size_t v, char *base);

/** str **/
int						ft_toupper(int c);
int						ft_tolower(int c);
int						ft_strcmp(const char *s1, const char *s2);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
char					*ft_strchr(const char *s, int c);
char					*ft_strrchr(const char *s, int c);
char					*ft_strnstr(const char *haystack, const char *needle,
							size_t len);
char					*ft_strdup(const char *s1);
char					*ft_substr(char const *s, unsigned int start,
							size_t len);
char					*ft_strjoin(char const *s1, char const *s2);
char					*ft_strtrim(char const *s1, char const *set);
char					**ft_split(char const *s, char c);
char					*ft_strmapi(char const *s, char (*f)(unsigned int,
								char));
void					ft_striteri(char *s, void (*f)(unsigned int, char *));
size_t					ft_strlen(const char *s);
char					*ft_strcat(char *dst, const char *src);
size_t					ft_strlcpy(char *dst, const char *src, size_t n);
size_t					ft_strlcat(char *dst, const char *src, size_t n);

/** write **/
size_t					ft_putchar_fd(char c, int fd);
size_t					ft_putstr_fd(char *s, int fd);
size_t					ft_putendl_fd(char *s, int fd);
size_t					ft_putnbr_fd(int n, int fd);

/** lib/ft_gnline **/
char					*ft_get_next_line(int fd);

/** lib/ft_printf **/
int						ft_printf(const char *format, ...);

/** ds/l-list **/
t_list					*ft_lstnew(void *content);
t_list					*ft_lstlast(t_list *lst);
t_list					*ft_lstmap(t_list *lst, void *(*f)(void *),
							void (*del)(void *));
int						ft_lstsize(t_list *lst);
void					ft_lstadd_front(t_list **lst, t_list *node);
void					ft_lstadd_back(t_list **lst, t_list *node);
void					ft_lstdelone(t_list *lst, void (*del)(void *));
void					ft_lstclear(t_list **lst, void (*del)(void *));
void					ft_lstiter(t_list *lst, void (*f)(void *));

/** ds/d-list **/
t_dlist					*ft_dlist_new(int id, t_dlist_node *node);
t_dlist_node			*ft_dlist_node_new(void *content);
void					ft_dlist_add_head(t_dlist *list, t_dlist_node *node);
void					ft_dlist_add_tail(t_dlist *list, t_dlist_node *node);
void					ft_dlist_rot(t_dlist *list);
void					ft_dlist_revrot(t_dlist *list);
void					ft_dlist_swap(t_dlist *list);
t_dlist_node			*ft_dlist_pop_head(t_dlist *list);
t_dlist_node			*ft_dlist_pop_tail(t_dlist *list);

/** ds/d-array **/
int						ft_array_init(t_array *array, size_t elem_size);
void					ft_array_free(t_array *array);
void					*ft_array_get(t_array *array, size_t index);
void					*ft_array_append(t_array *array, void *elem);

#endif
