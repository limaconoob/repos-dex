/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpepin <jpepin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/27 00:14:45 by jpepin            #+#    #+#             */
/*   Updated: 2016/12/27 00:23:56 by jpepin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>

char (*(ghoster)(void))[4096]
{ static char the[4096];
  bzero(the, 4096);
  return (&the); }

void start(void *cur_dir)
{ static char (*the)[4096];
  the = ghoster();
  getcwd(*the, 4096);
  *((unsigned char **)cur_dir) = *the; }

int main (void)
{ static unsigned char *tmp[1];
  start(tmp);
  printf("TMP::%s\n", *tmp); }
