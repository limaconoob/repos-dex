
#ifndef DEX_H
#define DEX_H
#include <time.h>

void icwd(void *dir);

typedef struct s_cdir
{ char stock[4096];
  char actual[4096];
  char message_bullets[1024];
  int collider;
  time_t current; } t_cdir;

#endif
