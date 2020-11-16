/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafflow <msafflow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 16:51:55 by msafflow          #+#    #+#             */
/*   Updated: 2020/11/16 23:03:25 by msafflow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		main(int argc, char **argv)
{
	char *cmd;

	while (1)
	{
		print_prompt1();
		cmd = read_cmd(cmd);
		if (!cmd)
		{
			exit(EXIT_SUCCESS);
		}
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
		write(1, cmd, ft_strlen(cmd));
		write(1, "\n", 1);
		free(cmd);
	}
	exit(EXIT_SUCCESS);
}

char	*read_cmd(char *buf)
{
	int		buflen;
	char	*tmp;

	get_next_line(0, &buf);
	buflen = ft_strlen(buf);
	while (buflen > 1 && buf[buflen - 1] == '\\')
	{
		buf[buflen - 1] = '\n';
		buf[buflen] = '\0';
		print_prompt2();
		tmp = buf;
		get_next_line(0, &buf);
		buf = ft_strjoin(tmp, buf);
		buflen = ft_strlen(buf);
	}
	return (buf);
}
