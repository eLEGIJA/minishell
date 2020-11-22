/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafflow <msafflow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 20:50:16 by msafflow          #+#    #+#             */
/*   Updated: 2020/11/19 19:45:24 by msafflow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "shell.h"
#include "node.h"
#include "parser.h"

t_node	*new_node(enum e_node_type type)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	memset(node, 0, sizeof(t_node));
	node->type = type;
	return (node);
}

void	add_child_node(t_node *parent, t_node *child)
{
	t_node	*sibling;

	if (!parent || !child)
		return ;
	if (!parent->first_child)
		parent->first_child = child;
	else
	{
		sibling = parent->first_child;
		while (sibling->next_sibling)
			sibling = sibling->next_sibling;
		sibling->next_sibling = child;
		child->prev_sibling = sibling;
	}
	parent->children++;
}

void	set_node_val_str(t_node *node, char *val)
{
	char	*val2;

	node->val_type = VAL_STR;
	if (!val)
		node->val.str = NULL;
	else
	{
		val2 = malloc(ft_strlen(val) + 1);
		if (!val2)
			node->val.str = NULL;
		else
		{
			strcpy(val2, val);
			node->val.str = val2;
		}
	}
}

void	free_node_tree(t_node *node)
{
	t_node	*child;
	t_node	*next;

	if (!node)
		return ;
	child = node->first_child;
	while (child)
	{
		next = child->next_sibling;
		free_node_tree(child);
		child = next;
	}
	if (node->val_type == VAL_STR)
		if (node->val.str)
			free(node->val.str);
	free(node);
}
