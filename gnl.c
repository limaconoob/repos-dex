/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpepin <jpepin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/03 05:47:22 by jpepin            #+#    #+#             */
/*   Updated: 2017/01/15 10:53:45 by jpepin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "neko.h"
#include "outils.h"
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>

#include <stdio.h>
#include <unistd.h>

# define BUFF_SIZE 1024

static char			*ft_norm(char *s1, int pot)
{
	int				i;
	char			*str;

	i = 0;
	if (!(str = (char*)malloc(LEN(s1) - pot)))
		return (NULL);
	while (s1[i + pot + 1] && (str[i] = s1[i + pot + 1]))
		i++;
	str[i] = '\0';
	return (str);
}

static char			*ft_concat(char *s1, char *s2, int pot)
{
	int				i;
	int				k;
	char			*str;

	str = NULL;
	i = 0;
	k = 0;
	if ((!s1 || !(*s1)) && s2)
		return (SUB(s2, 0, pot));
	if (!s2 && s1[pot])
		str = ft_norm(s1, pot);
	else if (s2)
	{
		if (!(str = (char*)malloc(LEN(s1) + pot + 1)))
			return (NULL);
		while (s1[i] && (str[i] = s1[i]))
			i++;
		while (s2[k] && (str[k + i] = s2[k]) && k < pot)
			k++;
		str[i + k] = '\0';
	}
	free(s1);
	return (str);
}

static int			ft_norm2(char **line, char *buf, int ret)
{
	int				i;

	i = 0;
	while (buf[i] != '\n' && i < ret)
		i++;
	*line = ft_concat(*line, buf, i);
	return (i);
}

static char			*ft_norm3(char **line, char *get)
{
	int				i;

	i = 0;
	while (get[i] && get[i] != '\n')
		i++;
	*line = ft_concat(NULL, get, i);
	return (ft_concat(get, NULL, i));
}

int					GNL(int fd, char **line)
{
	static char		*get = NULL;
	int				ret;
	int				i;
	char			buf[BUFF_SIZE];

	if (!line || read(fd, NULL, 0) == -1)
		return (-1);
	*line = NULL;
	i = 0;
	if (get && *get && (get = ft_norm3(line, get)))
		return (1);
	while ((ret = read(fd, buf, BUFF_SIZE)) > 0)
	{
		i = ft_norm2(line, buf, ret);
		if (i + 1 < ret && (get = ft_concat(get, &buf[i] + 1, ret - i - 1)))
			return (1);
		else if (buf[i] == '\n')
			return (1);
	}
	return (*line ? 1 : ret);
}
