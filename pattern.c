/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafflow <msafflow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 22:41:28 by msafflow          #+#    #+#             */
/*   Updated: 2020/11/28 21:54:20 by msafflow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <regex.h>
#include <fnmatch.h>
#include <locale.h>
#include <glob.h>
#include <sys/stat.h>
#include "shell.h"

/*
** check if the string *p has any regular expression (regex) characters,
** which are *, ?, [ and ].
*/

int				has_glob_chars(char *p, size_t len)
{
	char		*p2;
	char		ob;
	char		cb;

	p2 = p + len;
	ob = 0;
	cb = 0;
	while (p < p2 && *p)
	{
		if (*p == '*' || *p == '?')
			return (1);
		if (*p == '[')
			ob++;
		if (*p == ']')
			cb++;
		p++;
	}
	if (ob && ob == cb)
		return (1);
	return (0);
}

/*
** find the shortest or longest prefix of str that matches
** pattern, depending on the value of longest.
** return value is the index of 1 after the last character
** in the prefix, i.e. where you should put a '\0' to get
** the prefix.
*/

int				match_prefix(char *pattern, char *str, int longest)
{
	char		*lmatch;
	char		*smatch;
	char		c;
	char		*s;

	if (!pattern || !str)
		return (0);
	s = str + 1;
	c = *s;
	smatch = NULL;
	lmatch = NULL;
	while (c)
	{
		*s = '\0';
		if (fnmatch(pattern, str, 0) == 0)
		{
			if (!smatch)
			{
				if (!longest)
				{
					*s = c;
					return (s - str);
				}
				smatch = s;
			}
			lmatch = s;
		}
		*s = c;
		c = *(++s);
	}
	if (lmatch)
		return (lmatch - str);
	if (smatch)
		return (smatch - str);
	return (0);
}

/*
** find the shortest or longest suffix of str that matches
** pattern, depending on the value of longest.
** return value is the index of the first character in the
** matched suffix.
*/

int				match_suffix(char *pattern, char *str, int longest)
{
	char		*s;
	char		*smatch;
	char		*lmatch;

	if (!pattern || !str)
		return (0);
	s = str + ft_strlen(str) - 1;
	smatch = NULL;
	lmatch = NULL;
	while (s > str)
	{
		if (fnmatch(pattern, str, 0) == 0)
		{
			if (!smatch)
			{
				if (!longest)
					return (s - str);
				smatch = s;
			}
			lmatch = s;
		}
		s--;
	}
	if (lmatch)
		return (lmatch - str);
	if (smatch)
		return (smatch - str);
	return (0);
}

/*
** perform pathname (or filename) expansion, matching files in the given
** *dir to the given *path, which is treated as a regex pattern that
** specifies which filename(s) we should match.
** returns a char ** pointer to the
** list of matched filenames, or NULL if nothing matched.
*/

char			**get_filename_matches(char *pattern, glob_t *matches)
{
	int			res;

	matches->gl_pathc = 0;
	matches->gl_pathv = NULL;
	if (!pattern)
		return (NULL);
	res = glob(pattern, 0, NULL, matches);
	if (res != 0)
	{
		globfree(matches);
		return (NULL);
	}
	return (matches->gl_pathv);
}
