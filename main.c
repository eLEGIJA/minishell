/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafflow <msafflow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 16:51:55 by msafflow          #+#    #+#             */
/*   Updated: 2020/11/22 19:07:15 by msafflow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "source.h"
#include "parser.h"
#include "executor.h"
#include "scanner.h"

int		main(int argc, char **argv)
{
	char		*cmd;
	t_source	src;

	while (1)
	{
		print_prompt1();
		if (!(cmd = read_cmd(cmd)))
			exit(EXIT_SUCCESS);
		if (cmd[0] == '\0' || strcmp(cmd, "\n") == 0)
		{
			free(cmd);
			continue;
		}
		if (strcmp(cmd, "exit") == 0)
		{
			free(cmd);
			break ;
		}
		src.buffer = cmd;
		src.bufsize = ft_strlen(cmd);
		src.curpos = INIT_SRC_POS;
		parse_and_execute(&src);
		free(cmd);
	}
	exit(EXIT_SUCCESS);
}

char	*read_cmd(char *buf)
{
	int			buflen;
	char		*tmp;

	get_next_line(0, &buf);
	buflen = ft_strlen(buf);
	while (buflen > 1 && buf[buflen - 1] == '\\')
	{
		buf[buflen - 1] = '\0';
		print_prompt2();
		tmp = buf;
		get_next_line(0, &buf);
		buf = ft_strjoin(tmp, buf);
		buflen = ft_strlen(buf);
	}
	return (buf);
}

int		parse_and_execute(t_source *src)
{
	t_token		*tok;
	t_node		*cmd;

	skip_white_spaces(src);
	tok = tokenize(src);
	if (tok == &g_eof_token)
		return (0);
	while (tok && tok != &g_eof_token)
	{
		cmd = parse_simple_command(tok);
		if (!cmd)
			break ;
		do_simple_command(cmd);
		free_node_tree(cmd);
		tok = tokenize(src);
	}
	return (1);
}
