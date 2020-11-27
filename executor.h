/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafflow <msafflow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 21:08:35 by msafflow          #+#    #+#             */
/*   Updated: 2020/11/27 18:29:33 by msafflow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "node.h"

char	*search_path(char *file);
int		do_exec_cmd(int argc, char **argv);
int		do_simple_command(t_node *node);

#endif
