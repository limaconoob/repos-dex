
#include "neko.h"
#include "outils.h"
#include "dex.h"

void start(t_lbstat *lib, void **data)
{ (void)lib;
  static t_cdir tmp[1];
  icwd(&((*tmp).stock));
  BZE((*tmp).message_bullets, 1024);
  (*tmp).current = time(NULL);
  (*tmp).collider = 0;
  *data = &tmp; }
