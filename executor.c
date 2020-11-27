/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafflow <msafflow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 18:28:37 by msafflow          #+#    #+#             */
/*   Updated: 2020/11/27 19:38:39 by msafflow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "shell.h"
#include "node.h"
#include "executor.h"

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

int do_simple_command(t_node *node)
{
    if(!node)
    {
        return 0;
    }

    t_node *child = node->first_child;
    if(!child)
    {
        return 0;
    }
    
    int argc = 0;           /* arguments count */
    int targc = 0;          /* total alloc'd arguments count */
    char **argv = NULL;
    char *str;

    while(child)
    {
        str = child->val.str;
        /*perform word expansion */
        t_word *w = word_expand(str);
        
        /* word expansion failed */
        if(!w)
        {
            child = child->next_sibling;
            continue;
        }

        /* add the words to the arguments list */
        t_word *w2 = w;
        while(w2)
        {
            if(check_buffer_bounds(&argc, &targc, &argv))
            {
                str = malloc(strlen(w2->data)+1);
                if(str)
                {
                    strcpy(str, w2->data);
                    argv[argc++] = str;
                }
            }
            w2 = w2->next;
        }
        
        /* free the memory used by the expanded words */
        free_all_words(w);
        
        /* check the next word */
        child = child->next_sibling;
    }

    /* even if arc == 0, we need to alloc memory for argv */
    if(check_buffer_bounds(&argc, &targc, &argv))
    {
        /* NULL-terminate the array */
        argv[argc] = NULL;
    }

    int i = 0;
    for( ; i < g_builtins_count; i++)
    {
        if(strcmp(argv[0], g_builtins[i].name) == 0)
        {
            g_builtins[i].func(argc, argv);
            free_buffer(argc, argv);
            return 1;
        }
    }

    pid_t child_pid = 0;
    if((child_pid = fork()) == 0)
    {
        do_exec_cmd(argc, argv);
        fprintf(stderr, "error: failed to execute command: %s\n", strerror(errno));
        if(errno == ENOEXEC)
        {
            exit(126);
        }
        else if(errno == ENOENT)
        {
            exit(127);
        }
        else
        {
            exit(EXIT_FAILURE);
        }
    }
    else if(child_pid < 0)
    {
        fprintf(stderr, "error: failed to fork command: %s\n", strerror(errno));
	free_buffer(argc, argv);
        return 0;
    }

    int status = 0;
    waitpid(child_pid, &status, 0);
    free_buffer(argc, argv);
    
    return 1;
}
