/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafflow <msafflow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 20:20:34 by msafflow          #+#    #+#             */
/*   Updated: 2020/11/18 23:31:23 by msafflow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "shell.h"
# include "scanner.h"
# include "node.h"
# include "source.h"

t_node		*parse_simple_command(t_token *tok);

#endif
