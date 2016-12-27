
#include "neko.h"
#include "outils.h"
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Descritpion du module repos-dex:
//  start : initialise le dossier courant
//   idle : Si on change de dossier courant et qu'il y a présence
//          d'un `.git` &| d'un `README.mb` &| d'un `Makefile`,
//          ça va extraire des infos de ces fichiers :
//            - Git: status, branches, nom du répo, nom du propriétaire
//            - Readme: Affiche les noms des chapitres `#`
//            - Makefile: Affiche les règles du Makefile et $NAME
//          + Donne le pourcentage des extensions (par langage)

void openner(char *coucou, char *file, char *concat)
{ int i = 0;
  BZE(concat, 4096);
  if (coucou)
  { while (i < 4096 && *coucou)
    { concat[i] = *coucou;
      coucou += 1;
      i += 1; }
    if (i < 4096)
    { concat[i] = '/';
      i += 1; }}
  if (file)
  { while (i < 4096 && *file)
    { concat[i] = *file;
      file += 1;
      i += 1; }}}

void check_dir(t_lbstat *lib, char *coucou)
{ DIR *dir = opendir(coucou);
  char path[4096];
  int fd;
  if (dir)
  { struct dirent *dp;
    while ((dp = readdir(dir)))
    { if (NCMP("README.md", dp->d_name, 9) == 0 && !dp->d_name[9])
      { openner(coucou, dp->d_name, path);
        printf("OPENNER::%s\n", path);
        fd = open(path, O_RDONLY);
        close(fd); }
      else if (NCMP("Makefile", dp->d_name, 8) == 0 && !dp->d_name[8])
      { openner(coucou, dp->d_name, path);
        printf("OPENNER::%s\n", path);
        fd = open(path, O_RDONLY);
        close(fd); }}}}

void start(t_lbstat *lib, void *cur_dir)
{ (void)lib;
  char the[4096];
  BZE(the, 4096);
  getcwd(the, 4096);
  NCPY((char *)cur_dir, the, 4096); }

void idle(t_lbstat *lib, void *cur_dir)
{ char (*tmp1)[4096] = (char (*)[4096])(cur_dir);
  char (*tmp2)[4096] = (char (*)[4096])((unsigned long)cur_dir + 4096);
  start(lib, tmp2);
  if (NCMP(*tmp1, *tmp2, 4096))
  { start(lib, tmp1);
    check_dir(lib, *tmp1); }}

int main(void)
{ t_lbstat *lib;
  char tmp[2][4096];
  BZE(tmp[0], 4096);
  BZE(tmp[1], 4096);
  start(lib, &(tmp[0]));
  chdir("/Users/jpepin/Ausbildung");
  idle(lib, &tmp); }
