/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   source.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafflow <msafflow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 23:26:25 by msafflow          #+#    #+#             */
/*   Updated: 2020/11/17 00:17:36 by msafflow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOURCE_H
# define SOURCE_H

# define EOF			(-1)
# define ERRCHAR		( 0)
# define INIT_SRC_POS	(-2)

struct		s_source
{
	char	*buffer;
	long	bufsize;
	long	curpos;
}			t_source;

char		next_char(struct source_s *src);
void		unget_char(struct source_s *src);
char		peek_char(struct source_s *src);
void		skip_white_spaces(struct source_s *src);

#endif
