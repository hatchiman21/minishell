/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 03:59:10 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/24 21:18:51 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 64
# endif

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

typedef struct s_line_list
{
	int					fd;
	char				*buff;
	struct s_line_list	*next;
}	t_line_list;

int				ft_isascii(int c);
int				ft_isprint(int c);
int				ft_isalpha(int c);
int				ft_isdigit(int c);
int				ft_isalnum(int c);

int				ft_tolower(int c);
int				ft_toupper(int c);
size_t			ft_strlen(const char *s);
size_t			ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t			ft_strlcat(char *dst, const char *src, size_t dstsize);

char			*ft_strchr(const char *s, int c);
char			*ft_strrchr(const char *s, int c);
char			*ft_strnstr(const char *haystack,
					const char *needle, size_t len);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
long int		ft_atoi(const char *str);
unsigned int	ft_atoi_base(const char *str, int base);

void			*ft_memset(void *b, int c, size_t len);
void			ft_bzero(void *s, size_t n);
void			*ft_memcpy(void *dst, const void *src, size_t n);
void			*ft_memccpy(void *dst, const void *src, int c, size_t n);
void			*ft_memmove(void *dst, const void *src, size_t len);

void			*ft_memchr(const void *s, int c, size_t n);
int				ft_memcmp(const void *s1, const void *s2, size_t n);
char			*ft_strdup(const char *s1);
void			*ft_calloc(size_t count, size_t size);
void			ft_striteri(char *s, void (*f)(unsigned int, char*));

char			*ft_itoa(int n);
char			*ft_itoa_hex(unsigned int n, int base);
int				ft_putchar_fd(char c, int fd);
int				ft_putstr_fd(char *s, int fd);
void			ft_putendl_fd(char *s, int fd);
int				ft_putnbr_fd(long n, int fd);

char			*ft_substr(char const *s, unsigned int start, size_t len);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_strtrim(char const *s1, char const *set);
char			**ft_split(char const *s, char c);
char			*ft_strmapi(char const *s, char (*f)(unsigned int, char));

t_list			*ft_lstlast(t_list *lst);
t_list			*ft_lstnew(void *content);
void			ft_lstadd_front(t_list **lst, t_list *new);
int				ft_lstsize(t_list *lst);
void			ft_lstadd_back(t_list **lst, t_list *new);

void			ft_lstdelone(t_list *lst, void (*del)(void*));
void			ft_lstclear(t_list **lst, void (*del)(void*));
void			ft_lstiter(t_list *lst, void (*f)(void *));
t_list			*ft_lstmap(t_list *lst,
					void *(*f)(void *), void (*del)(void *));

int				ft_putnbr_hex_fd(unsigned long n, int fd, int j);
int				ft_printf(const char *string, ...);
int				ft_dprintf(int fd, const char *string, ...);

int				ft_strchr_find(const char *s, int c);
char			*ft_lstclear_item(t_line_list **lst, int fd);
char			*get_next_line(int fd);

#endif
