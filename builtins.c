/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafflow <msafflow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 20:38:29 by msafflow          #+#    #+#             */
/*   Updated: 2020/11/25 20:51:07 by msafflow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_builtin	g_builtins[] =
{
	{"dump", dump},
};

int			g_builtins_count = sizeof(g_builtins) / sizeof(t_builtin);
