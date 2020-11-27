/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafflow <msafflow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 16:53:05 by msafflow          #+#    #+#             */
/*   Updated: 2020/11/27 19:59:22 by msafflow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "symtab/symtab.h"

void	print_prompt1(void)
{
	t_symtab_entry	*entry;

	entry = get_symtab_entry("PS1");
	if (entry && entry->val)
		fprintf(stderr, "%s", entry->val);
	else
		fprintf(stderr, "$ ");
}

void	print_prompt2(void)
{
	t_symtab_entry	*entry;

	entry = get_symtab_entry("PS2");
	if (entry && entry->val)
		fprintf(stderr, "%s", entry->val);
	else
		fprintf(stderr, "> ");
}
