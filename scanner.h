/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafflow <msafflow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 17:30:15 by msafflow          #+#    #+#             */
/*   Updated: 2020/11/27 18:30:43 by msafflow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCANNER_H
# define SCANNER_H

# include "source.h"
# include "node.h"

/*
** source of input
** length of token text
** token text
*/

typedef struct		s_token
{
	struct s_source	*src;
	int				text_len;
	char			*text;
}					t_token;

/*
** the special EOF token, which indicates the end of input
*/

extern t_token		g_eof_token;
t_token				*tokenize(t_source *src);
void				free_token(t_token *tok);

#endif
