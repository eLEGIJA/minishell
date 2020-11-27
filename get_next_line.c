/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafflow <msafflow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 16:55:28 by msafflow          #+#    #+#             */
/*   Updated: 2020/11/27 19:53:14 by msafflow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

char	*ft_strdup(const char *s)
{
	char	*str;
	char	*str2;
	int		i;
	int		j;

	i = 0;
	j = 0;
	str2 = (char *)s;
	while (str2[i] != '\0')
		i++;
	str = (char*)malloc(sizeof(*str2) * (i + 1));
	if (!str)
		return (NULL);
	while (j < i)
	{
		str[j] = str2[j];
		j++;
	}
	str[j] = '\0';
	return (str);
}

size_t	ft_strlen(const char *s)
{
	size_t len;

	len = 0;
	while (s[len])
	{
		len++;
	}
	return (len);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	y;
	size_t	i;
	char	*str;

	i = 0;
	y = 0;
	if (!s1 || !s2)
		return (NULL);
	if (!(str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1))))
		return (NULL);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[y])
		str[i++] = s2[y++];
	str[i] = '\0';
	return (str);
}

int		get_next_line(int fd, char **line)
{
	char	buf[2];
	int		sr;
	char	*tmp;

	sr = 0;
	buf[1] = '\0';
	if (!line)
		return (-1);
	if (!(*line = malloc(1)))
		return (-1);
	**line = 0;
	while ((sr = read(fd, buf, 1)) > 0)
	{
		if (*buf != '\n' && *buf != EOF && *buf != '\0')
		{
			tmp = *line;
			*line = ft_strjoin(*line, buf);
			free(tmp);
		}
		else
			break ;
	}
	return (sr);
}
