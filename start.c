
#include "neko.h"
#include "outils.h"
#include "dex.h"

void start(t_lbstat *lib, void **data)
{ (void)lib;
  static t_cdir tmp[1];
  BZE(((*tmp).stock), 4096);
  BZE(((*tmp).actual), 4096);
  BZE((*tmp).message_bullets, 1024);
  *data = &tmp; }
