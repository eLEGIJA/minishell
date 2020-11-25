/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symtab.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafflow <msafflow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 15:00:45 by msafflow          #+#    #+#             */
/*   Updated: 2020/11/25 22:50:30 by msafflow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shell.h"
#include "node.h"
#include "parser.h"
#include "symtab.h"

t_symtab_stack		g_symtab_stack;
int					g_symtab_level;

void				init_symtab(void)
{
	t_symtab		*global_symtab;

	g_symtab_stack.symtab_count = 1;
	g_symtab_level = 0;
	global_symtab = malloc(sizeof(t_symtab));
	if (!global_symtab)
	{
		fprintf(stderr, "fatal error: no memory for global symbol table\n");
		exit(EXIT_FAILURE);
	}
	memset(global_symtab, 0, sizeof(t_symtab));
	g_symtab_stack.global_symtab = global_symtab;
	g_symtab_stack.local_symtab = global_symtab;
	g_symtab_stack.symtab_list[0] = global_symtab;
	global_symtab->level = 0;
}

t_symtab			*new_symtab(int level)
{
	t_symtab		*symtab;

	symtab = malloc(sizeof(t_symtab));
	if (!symtab)
	{
		fprintf(stderr, "fatal error: no memory for new symbol table\n");
		exit(EXIT_FAILURE);
	}
	memset(symtab, 0, sizeof(t_symtab));
	symtab->level = level;
	return (symtab);
}

void				free_symtab(t_symtab *symtab)
{
	t_symtab_entry	*entry;
	t_symtab_entry	*next;

	if (symtab == NULL)
		return ;
	entry = symtab->first;
	while (entry)
	{
		if (entry->name)
			free(entry->name);
		if (entry->val)
			free(entry->val);
		if (entry->func_body)
			free_node_tree(entry->func_body);
		next = entry->next;
		free(entry);
		entry = next;
	}
	free(symtab);
}

void				dump_local_symtab(void)
{
	t_symtab		*symtab;
	t_symtab_entry	*entry;
	int				i;
	int				indent;

	symtab = g_symtab_stack.local_symtab;
	i = 0;
	indent = symtab->level * 4;
	fprintf(stderr, "%*sSymbol table [Level %d]:\r\n", indent, " ", symtab->level);
	fprintf(stderr, "%*s===========================\r\n", indent, " ");
	fprintf(stderr, "%*s  No               Symbol                    Val\r\n", indent, " ");
	fprintf(stderr, "%*s------ -------------------------------- ------------\r\n", indent, " ");
	entry = symtab->first;
	while (entry)
	{
		fprintf(stderr, "%*s[%04d] %-32s '%s'\r\n", indent, " ",
				i++, entry->name, entry->val);
		entry = entry->next;
	}
	fprintf(stderr, "%*s------ -------------------------------- ------------\r\n", indent, " ");
}

t_symtab_entry		*add_to_symtab(char *symbol)
{
	t_symtab_entry	*entry;
	t_symtab		*st;

	if (!symbol || symbol[0] == '\0')
		return (NULL);
	st = g_symtab_stack.local_symtab;
	entry = NULL;
	if ((entry = do_lookup(symbol, st)))
		return (entry);
	entry = malloc(sizeof(t_symtab_entry));
	if (!entry)
	{
		fprintf(stderr, "fatal error: no memory for new symbol table entry\n");
		exit(EXIT_FAILURE);
	}
	memset(entry, 0, sizeof(t_symtab_entry));
	entry->name = malloc(strlen(symbol) + 1);
	if (!entry->name)
	{
		fprintf(stderr, "fatal error: no memory for new symbol table entry\n");
		exit(EXIT_FAILURE);
	}
	strcpy(entry->name, symbol);
	if (!st->first)
	{
		st->first = entry;
		st->last = entry;
	}
	else
	{
		st->last->next = entry;
		st->last = entry;
	}
	return (entry);
}

t_symtab_entry		*do_lookup(char *str, t_symtab *symtable)
{
	t_symtab_entry	*entry;

	if (!str || !symtable)
		return (NULL);
	entry = symtable->first;
	while (entry)
	{
		if (strcmp(entry->name, str) == 0)
			return (entry);
		entry = entry->next;
	}
	return (NULL);
}

t_symtab_entry		*get_symtab_entry(char *str)
{
	int				i;
	t_symtab		*symtab;
	t_symtab_entry	*entry;

	i = g_symtab_stack.symtab_count - 1;
	symtab = g_symtab_stack.symtab_list[i];
	entry = do_lookup(str, symtab);
	if (entry)
		return (entry);
	while (--i >= 0)
	{
		symtab = g_symtab_stack.symtab_list[i];
		entry = do_lookup(str, symtab);
		if (entry)
			return (entry);
	}
	return (NULL);
}

void				symtab_entry_setval(t_symtab_entry *entry, char *val)
{
	char			*val2;

	if (entry->val)
		free(entry->val);
	if (!val)
		entry->val = NULL;
	else
	{
		val2 = malloc(ft_strlen(val) + 1);
		if (val2)
			strcpy(val2, val);
		else
			fprintf(stderr, "error: no memory for symbol table entry's value\n");
		entry->val = val2;
	}
}

int					rem_from_symtab(t_symtab_entry *entry, t_symtab *symtab)
{
	int res = 0;
	if (entry->val)
		free(entry->val);
	if (entry->func_body)
		free_node_tree(entry->func_body);
	free(entry->name);
	if (symtab->first == entry)
	{
		symtab->first = symtab->first->next;
		if (symtab->last == entry)
			symtab->last = NULL;
		res = 1;
	}
	else
	{
		t_symtab_entry *e = symtab->first;
		t_symtab_entry *p = NULL;
		while (e && e != entry)
		{
			p = e;
			e = e->next;
		}
		if (e == entry)
		{
			p->next = entry->next;
			res = 1;
		}
	}
	free(entry);
	return (res);
}

void				symtab_stack_add(t_symtab *symtab)
{
	g_symtab_stack.symtab_list[g_symtab_stack.symtab_count++] = symtab;
	g_symtab_stack.local_symtab = symtab;
}

t_symtab			*symtab_stack_push(void)
{
	t_symtab		*st;

	st = new_symtab(++g_symtab_level);
	symtab_stack_add(st);
	return (st);
}

t_symtab			*symtab_stack_pop(void)
{
	t_symtab		*st;

	if (g_symtab_stack.symtab_count == 0)
		return (NULL);
	st = g_symtab_stack.symtab_list[g_symtab_stack.symtab_count - 1];
	g_symtab_stack.symtab_list[--g_symtab_stack.symtab_count] = NULL;
	g_symtab_level--;
	if (g_symtab_stack.symtab_count == 0)
	{
		g_symtab_stack.local_symtab = NULL;
		g_symtab_stack.global_symtab = NULL;
	}
	else
		g_symtab_stack.local_symtab = \
			g_symtab_stack.symtab_list[g_symtab_stack.symtab_count - 1];
	return (st);
}

t_symtab			*get_local_symtab(void)
{
	return (g_symtab_stack.local_symtab);
}

t_symtab			*get_global_symtab(void)
{
	return (g_symtab_stack.global_symtab);
}

t_symtab_stack		*get_symtab_stack(void)
{
	return (&g_symtab_stack);
}
