
#ifndef DEX_H
#define DEX_H

void icwd(void *dir);

typedef struct s_cdir
{ char stock[4096];
  char actual[4096];
  char message_bullets[1024]; } t_cdir;

#endif
