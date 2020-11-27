/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initsh.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafflow <msafflow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 20:25:09 by msafflow          #+#    #+#             */
/*   Updated: 2020/11/27 19:22:29 by msafflow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "shell.h"
#include "symtab/symtab.h"

void		initsh_utils(int len, t_symtab_entry *entry, char **p2, char *eq)
{
	char name[len + 1];

	strncpy(name, *p2, len);
	name[len] = '\0';
	entry = add_to_symtab(name);
	if (entry)
	{
		symtab_entry_setval(entry, eq + 1);
		entry->flags |= FLAG_EXPORT;
	}
}

void		initsh(char **env)
{
	t_symtab_entry	*entry;
	char			**p2;
	char			*eq;
	int				len;

	p2 = env;
	init_symtab();
	while (*p2)
	{
		eq = strchr(*p2, '=');
		if (eq)
		{
			len = eq - (*p2);
			initsh_utils(len, entry, p2, eq);
		}
		else
			entry = add_to_symtab(*p2);
		p2++;
	}
	entry = add_to_symtab("PS1");
	symtab_entry_setval(entry, "$ ");
	entry = add_to_symtab("PS2");
	symtab_entry_setval(entry, "> ");
}
