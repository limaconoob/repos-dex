/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpepin <jpepin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/27 00:14:45 by jpepin            #+#    #+#             */
/*   Updated: 2016/12/27 00:30:32 by jpepin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>

void start(void *cur_dir)
{ char the[4096];
  bzero(the, 4096);
  getcwd(the, 4096);
  *((char **)cur_dir) = the; }

int main (void)
{ unsigned char *tmp[1];
  start(tmp);
  printf("TMP::%s\n", *tmp); }
