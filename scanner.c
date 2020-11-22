/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafflow <msafflow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 17:33:27 by msafflow          #+#    #+#             */
/*   Updated: 2020/11/21 17:52:17 by msafflow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "shell.h"
#include "scanner.h"
#include "source.h"

char	*g_tok_buf = NULL;
int		g_tok_bufsize = 0;
int		g_tok_bufindex = -1;

/*
** special token to indicate end of input
*/

t_token		g_eof_token =
{
	.text_len = 0,
};

void		add_to_buf(char c)
{
	char	*tmp;

	g_tok_buf[g_tok_bufindex++] = c;
	if (g_tok_bufindex >= g_tok_bufsize)
	{
		tmp = realloc(g_tok_buf, g_tok_bufsize * 2);
		if (!tmp)
		{
			errno = ENOMEM;
			return ;
		}
		g_tok_buf = tmp;
		g_tok_bufsize *= 2;
	}
}

t_token		*create_token(char *str)
{
	t_token	*tok;
	char	*nstr;

	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	memset(tok, 0, sizeof(t_token));
	tok->text_len = strlen(str);
	nstr = malloc(tok->text_len + 1);
	if (!nstr)
	{
		free(tok);
		return (NULL);
	}
	strcpy(nstr, str);
	tok->text = nstr;
	return (tok);
}

void		free_token(t_token *tok)
{
	if (tok->text)
		free(tok->text);
	free(tok);
}

int			check_token(t_source *src)
{
	if (!src || !src->buffer || !src->bufsize)
	{
		errno = ENODATA;
		return (0);
	}
	if (!g_tok_buf)
	{
		g_tok_bufsize = 1024;
		g_tok_buf = malloc(g_tok_bufsize);
		if (!g_tok_buf)
		{
			errno = ENOMEM;
			return (0);
		}
	}
	g_tok_bufindex = 0;
	g_tok_buf[0] = '\0';
	return (1);
}

void		tok_switch(char *nc, int *endloop, t_source *src)
{
	if (*nc == '\t' || *nc == ' ')
	{
		if (g_tok_bufindex > 0)
			*endloop = 1;
		return ;
	}
	else if (*nc == '\n')
	{
		if (g_tok_bufindex > 0)
			unget_char(src);
		else
			add_to_buf(*nc);
		*endloop = 1;
		return ;
	}
	else
	{
		add_to_buf(*nc);
		return ;
	}
}

int			tok_while(t_source *src)
{
	int		endloop;
	char	nc;
	int		f;

	endloop = 0;
	f = 1;
	if (!(check_token(src)))
		return (0);
	nc = next_char(src);
	if (nc == ERRCHAR || nc == EOF)
		return (0);
	tok_switch(&nc, &endloop, src);
	if (endloop)
		f = 0;
	while (f && (nc = next_char(src)) != EOF)
	{
		tok_switch(&nc, &endloop, src);
		if (endloop)
			break ;
	}
	return (1);
}

t_token		*tokenize(t_source *src)
{
	t_token	*tok;

	if (!(tok_while(src)))
		return (&g_eof_token);
	if (g_tok_bufindex == 0)
		return (&g_eof_token);
	if (g_tok_bufindex >= g_tok_bufsize)
		g_tok_bufindex--;
	g_tok_buf[g_tok_bufindex] = '\0';
	tok = create_token(g_tok_buf);
	if (!tok)
	{
		fprintf(stderr, "error: failed to alloc buffer: %s\n",
				strerror(errno));
		return (&g_eof_token);
	}
	tok->src = src;
	return (tok);
}
