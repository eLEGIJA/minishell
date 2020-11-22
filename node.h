/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafflow <msafflow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 20:22:19 by msafflow          #+#    #+#             */
/*   Updated: 2020/11/19 20:51:05 by msafflow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NODE_H
# define NODE_H

/*
** simple command
** variable name (or simply, a word)
*/

enum					e_node_type
{
	NODE_COMMAND,
	NODE_VAR,
};

/*
** signed int
** unsigned int
** signed long long
** unsigned long long
** floating point
** variable name (or simply, a word)
** long double
** char
** str (char pointer)
*/

enum					e_val_type
{
	VAL_SINT = 1,
	VAL_UINT,
	VAL_SLLONG,
	VAL_ULLONG,
	VAL_FLOAT,
	VAL_LDOUBLE,
	VAL_CHR,
	VAL_STR,
};

union					u_symval
{
	long				sint;
	unsigned long		uint;
	long long			sllong;
	unsigned long long	ullong;
	double				sfloat;
	long double			ldouble;
	char				chr;
	char				*str;
};

/*
** type of this node
** type of this node's val field
** value of this node
** number of child nodes
** first child node
** if this is a child node, keep
** pointers to prev/next siblings
*/

typedef struct			s_node
{
	enum e_node_type	type;
	enum e_val_type		val_type;
	union u_symval		val;
	int					children;
	struct s_node		*first_child;
	struct s_node		*next_sibling;
	struct s_node		*prev_sibling;
}						t_node;

t_node					*new_node(enum e_node_type type);
void					add_child_node(t_node *parent, t_node *child);
void					free_node_tree(t_node *node);
void					set_node_val_str(t_node *node, char *val);

#endif
