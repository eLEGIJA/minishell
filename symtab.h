/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symtab.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafflow <msafflow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 00:25:57 by msafflow          #+#    #+#             */
/*   Updated: 2020/11/25 14:58:10 by msafflow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYMTAB_H
# define SYMTAB_H

# include "node.h"

# define MAX_SYMTAB	256

/*
** the type of a symbol table entry's value
*/

/*
** export entry to forked commands
*/

# define FLAG_EXPORT (1 << 0)

enum				e_symbol_type
{
	SYM_STR,
	SYM_FUNC,
};

/*
** the symbol table entry structure
*/

typedef struct				s_symtab_entry
{
	char					*name;
	enum e_symbol_type		val_type;
	char					*val;
	unsigned int			flags;
	struct s_symtab_entry	*next;
	struct node_s			*func_body;
}							t_symtab_entry;

/*
** the symbol table structure
*/

typedef struct				s_symtab
{
	int						level;
	t_symtab_entry			*first;
	t_symtab_entry			*last;
}							t_symtab;

/*
** values for the flags field of struct s_symtab_entry
*/

/*
** the symbol table stack structure
*/

typedef struct				s_symtab_stack
{
	int						symtab_count;
	struct s_symtab			*symtab_list[MAX_SYMTAB];
	struct s_symtab			*global_symtab;
	struct s_symtab			*local_symtab;
}							t_symtab_stack;

t_symtab					*new_symtab(int level);
t_symtab					*symtab_stack_push(void);
t_symtab					*symtab_stack_pop(void);
int							rem_from_symtab(t_symtab_entry *entry, \
								t_symtab *symtab);
t_symtab_entry				*add_to_symtab(char *symbol);
t_symtab_entry				*do_lookup(char *str, t_symtab *symtable);
t_symtab_entry				*get_symtab_entry(char *str);
t_symtab					*get_local_symtab(void);
t_symtab					*get_global_symtab(void);
t_symtab_stack				*get_symtab_stack(void);
void						init_symtab(void);
void						dump_local_symtab(void);
void						free_symtab(t_symtab *symtab);
void						symtab_entry_setval(t_symtab_entry *entry, \
								char *val);

#endif
