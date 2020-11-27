/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafflow <msafflow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 20:22:01 by msafflow          #+#    #+#             */
/*   Updated: 2020/11/27 18:30:20 by msafflow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "parser.h"
#include "node.h"
#include "source.h"

t_node	*parse_simple_command(t_token *tok)
{
	t_node		*cmd;
	t_source	*src;
	t_node		*word;
	int			i;

	cmd = new_node(NODE_COMMAND);
	if (!(parse_verification(tok, cmd)))
		return (NULL);
	src = tok->src;
	i = parse_util(tok, cmd);
	if (i == 0)
		return (NULL);
	else if (i == 1)
		return (cmd);
	while ((tok = tokenize(src)) != &g_eof_token)
	{
		i = parse_util(tok, cmd);
		if (i == 0)
			return (NULL);
		else if (i == 1)
			return (cmd);
	}
	return (cmd);
}

int		parse_util(t_token *tok, t_node *cmd)
{
	t_node		*word;

	if (tok->text[0] == '\n')
	{
		free_token(tok);
		return (1);
	}
	word = new_node(NODE_VAR);
	if (!word)
	{
		free_node_tree(cmd);
		free_token(tok);
		return (0);
	}
	set_node_val_str(word, tok->text);
	add_child_node(cmd, word);
	free_token(tok);
}

int		parse_verification(t_token *tok, t_node *cmd)
{
	if (!tok)
		return (NULL);
	if (!cmd)
	{
		free_token(tok);
		return (NULL);
	}
}
