/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafflow <msafflow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 21:11:06 by msafflow          #+#    #+#             */
/*   Updated: 2020/11/23 23:33:26 by msafflow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "node.h"
#include "executor.h"
#include "scanner.h"

int						search_utils2(char **p, char **p2, char *path, \
							struct stat	*st)
{
	if (!S_ISREG(st->st_mode))
	{
		errno = ENOENT;
		*p = *p2;
		if (**p2 == ':')
			(*p)++;
		return (1);
	}
	*p = malloc(ft_strlen(path) + 1);
	if (!*p)
		return (0);
	strcpy(*p, path);
	return (2);
}

int						search_utils(int len, char **p, char **p2, char **file)
{
	char		path[len + 2];
	struct stat	st;
	int			i;

	strncpy(path, *p, *p2 - *p);
	path[*p2 - *p] = '\0';
	if ((*p2)[-1] != '/')
		strcat(path, "/");
	strcat(path, *file);
	if (stat(path, &st) == 0)
	{
		return (search_utils2(p, p2, path, &st));
	}
	else
	{
		*p = *p2;
		if (**p2 == ':')
			(*p)++;
	}
	return (-1);
}

char					*search_path(char *file)
{
	char		*p;
	char		*p2;
	int			len[2];
	int			i;

	p = getenv("PATH");
	while (p && *p)
	{
		p2 = p;
		while (*p2 && *p2 != ':')
			p2++;
		len[0] = p2 - p;
		if (!len[0])
			len[0] = 1;
		len[1] = ft_strlen(file);
		i = search_utils(len[0] + len[1], &p, &p2, &file);
		if (i == 1)
			continue;
		if (i == 0)
			return (NULL);
		if (i == 2)
			return (p);
	}
	errno = ENOENT;
	return (NULL);
}

int						do_exec_cmd(int argc, char **argv)
{
	char		*path;

	if (strchr(argv[0], '/'))
	{
		execv(argv[0], argv);
	}
	else
	{
		path = search_path(argv[0]);
		if (!path)
			return (0);
		execv(path, argv);
		free(path);
	}
	return (0);
}

static inline void		free_argv(int argc, char **argv)
{
	if (!argc)
		return ;
	while (argc--)
		free(argv[argc]);
}

int						do_child(t_node *node, int *argc, char **argv[256])
{
	t_node		*child;
	char		*str;

	if (!node)
		return (0);
	child = node->first_child;
	if (!child)
		return (0);
	*argc = 0;
	while (child)
	{
		str = child->val.str;
		argv[*argc] = malloc(ft_strlen(str) + 1);
		if (!argv[*argc])
		{
			free_argv(*argc, argv);
			return (0);
		}
		strcpy(argv[*argc], str);
		if (++(*argc) >= 255)
			break ;
		child = child->next_sibling;
	}
	argv[*argc] = NULL;
	return (1);
}

int						do_simple_command(t_node *node)
{
	pid_t		child_pid;
	int			status;
	int			argc;
	char		*argv[256];

	if (!(do_child(node, &argc, &argv)))
		return (0);
	if ((child_pid = fork()) == 0)
	{
		do_exec_cmd(argc, argv);
		fprintf(stderr, "error: failed to execute command: %s\n",
				strerror(errno));
	}
	else if (child_pid < 0)
	{
		fprintf(stderr, "error: failed to fork command: %s\n",
			strerror(errno));
		return (0);
	}
	status = 0;
	waitpid(child_pid, &status, 0);
	free_argv(argc, argv);
	return (1);
}
