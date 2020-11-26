/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafflow <msafflow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 16:58:44 by msafflow          #+#    #+#             */
/*   Updated: 2020/11/26 21:59:14 by msafflow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include "source.h"

/*
** struct for builtin utilities
** utility name
** function to call to execute the utility
*/
typedef struct		s_builtin
{
	char			*name;
	int				(*func)(int argc, char **argv);
}					t_builtin;

/*
** the list of builtin utilities
*/
t_builtin			g_builtins[];

/*
** and their count
*/
extern int			g_builtins_count;

typedef struct		s_word
{
	char			*data;
	int				len;
	struct s_word	*next;
}					t_word;

int					parse_and_execute(t_source *src, char **env);
void				print_prompt1(void);
void				print_prompt2(void);
char				*read_cmd(char *buf);
int					get_next_line(int fd, char **line);
char				*ft_strjoin(char const *s1, char const *s2);
size_t				ft_strlen(const char *s);
char				*ft_strdup(const char *s);
void				ft_putchar_fd(char c, int fd);
void				initsh(char **env);
t_word				*make_word(char *str);
void				free_all_words(t_word *first);
char				*strchr_any(char *string, char *chars);
char				*quote_val(char *val, int add_quotes);
int					check_buffer_bounds(int *count, int *len, char ***buf);
void				free_buffer(int len, char **buf);

/*
** shell builtin utilities
*/
int					dump(int argc, char **argv);

#endif
