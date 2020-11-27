/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   source.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafflow <msafflow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 23:26:25 by msafflow          #+#    #+#             */
/*   Updated: 2020/11/27 18:31:28 by msafflow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOURCE_H
# define SOURCE_H

# define EOF			(-1)
# define ERRCHAR		( 0)
# define INIT_SRC_POS	(-2)

typedef struct	s_source
{
	char		*buffer;
	long		bufsize;
	long		curpos;
}				t_source;

char			next_char(t_source *src);
void			unget_char(t_source *src);
char			peek_char(t_source *src);
void			skip_white_spaces(t_source *src);

#endif
