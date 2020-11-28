/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafflow <msafflow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 16:58:44 by msafflow          #+#    #+#             */
/*   Updated: 2020/11/28 20:09:12 by msafflow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <glob.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include "source.h"
# include "executor.h"
# include "node.h"
# include "parser.h"
# include "scanner.h"

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
t_word				*make_word(char *word);
void				ree_all_words(t_word *first);
size_t				find_closing_quote(char *data);
size_t				find_closing_brace(char *data);
void				delete_char_at(char *str, size_t index);
char				*substitute_str(char *s1, char *s2, \
						size_t start, size_t end);
char				*wordlist_to_str(t_word *word);
t_word				*word_expand(char *orig_word);
char				*word_expand_to_str(char *word);
char				*tilde_expand(char *s);
char				*command_substitute(char *__cmd);
char				*var_expand(char *__var_name);
char				*pos_params_expand(char *tmp, int in_double_quotes);
t_word				*pathnames_expand(t_word *words);
t_word				*field_split(char *str);
void				remove_quotes(t_word *wordlist);
char				*arithm_expand(char *__expr);
char				*strchr_any(char *string, char *chars);
char				*quote_val(char *val, int add_quotes);
int					check_buffer_bounds(int *count, int *len, char ***buf);
void				free_buffer(int len, char **buf);
int					has_glob_chars(char *p, size_t len);
int					match_prefix(char *pattern, char *str, int longest);
int					match_suffix(char *pattern, char *str, int longest);
char				**get_filename_matches(char *pattern, glob_t *matches);
int					dump(int argc, char **argv);

#endif
