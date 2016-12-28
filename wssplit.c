
#include "neko.h"
#include "outils.h"
#include <stdlib.h>
#include <stdio.h>

static int		ft_count(char *str, char c)
{ int count = 0;
	while (*str)
	{ while (*str == c)
		{ str += 1; }
		if (*str)
		{ while (*str && *str != c)
			{ str += 1; }
			count += 1; }}
	return (count); }

static void		get_ttab(char **tab, char *str, char c)
{ int count = 0;
	int i;
	while (*str)
	{ while (*str == c)
		{ str += 1; }
		if (*str)
	  { i = 0;
			while (str[i] && str[i] != c)
			{ i += 1; }
			tab[count] = SUB(str, 0, i);
      count += 1;
			str += i; }}
	tab[count] = NULL; }

char			**SPL(char *str, char c)
{ char		**tab;
	if (!((tab = (char**)malloc(sizeof(char*) * (ft_count(str, c) + 1)))))
	{ return (NULL); }
	get_ttab(tab, str, c);
	return (tab); }
