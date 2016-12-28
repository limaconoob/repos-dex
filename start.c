
#include "neko.h"
#include "outils.h"
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
  //char *extension[13] = {"env", NULL};
  char path[4096];
  int fd;
  if (dir)
  { struct dirent *dp;
    while ((dp = readdir(dir)))
    { if (NCMP("README.md", dp->d_name, 9) == 0 && !dp->d_name[9])
      { openner(coucou, dp->d_name, path);
        printf("OPENNER::%s\n", path);
      //  SéCURITé
        fd = open(path, O_RDONLY);
        char **line;
       /* while (GNL(fd, line))
        { printf("LINE::%s\n", *line); } */
        while (GNL(fd, line))
        { if ((*line)[0] == '#' && (*line)[1] == ' ')
          { printf("LVL1::%s\n", &((*line)[2])); }
          else if ((*line)[0] == '#' && (*line)[1] == '#' && (*line)[2] == ' ')
          { printf("LVL2::%s\n", &((*line)[3])); }}
        close(fd); }
      else if (NCMP("Makefile", dp->d_name, 8) == 0 && !dp->d_name[8])
      { openner(coucou, dp->d_name, path);
        printf("OPENNER::%s\n", path);
      //  SéCURITé
        fd = open(path, O_RDONLY);
        char **line;
        while (GNL(fd, line))
        { int i = 0;
          while ((*line)[i])
          { if (i && ((*line)[i] == ' ' || !(*line)[i]) && (*line)[i - 1] == ':')
            { printf("REGLE::%s\n", *line); }
            else if ((*line)[i] == ' ')
            { break; }
            i += 1; }}
        close(fd); }
     /* else if (NCMP(".git", dp->d_name, 4) == 0 && !dp->d_name[4])
      { openner(coucou, ".git/logs/HEAD", path);
        printf("OPENNER::%s\n", path);
      //  SéCURITé
        fd = open(path, O_RDONLY);
        char **line;
        while (GNL(fd, line))
        { printf("LINE::%s\n", *line); }
        GNL(fd, line);
        printf("FD::%d\n", fd);
        printf("LINE::%s\n", *line);
        char **tab = SPL(*line, ' ');
        int len = 0;
        while (tab[len])
        { len += 1; }
        if (len >= 10)
        { time_t k = (time_t)atoi(tab[5]);
          printf("Nom::%s | Prénom::%s | Date::%s\n", tab[2], tab[3], ctime(&k)); }
        close(fd); }*/ }}}

typedef struct s_cdir
{ char stock[4096];
  char actual[4096]; } t_cdir;

void idle(t_lbstat *lib, void *cur_dir)
{ t_cdir *tmp = (t_cdir *)cur_dir;
  start(lib, &((*tmp).actual));
  if (NCMP((*tmp).stock, (*tmp).actual, 4096))
  { start(lib, &((*tmp).stock));
    check_dir(lib, (*tmp).actual); }}

void start(t_lbstat *lib, void *cur_dir)
{ (void)lib;
  unsigned long *bonjour = (unsigned long *)cur_dir;
  t_cdir tmp[1];
  BZE((*tmp).stock, 4096);
  BZE((*tmp).actual, 4096);
  getcwd((*tmp).stock, 4096);
  *bonjour = &tmp; }

printf("2 DATA::%lu, SIZE::%lu\n", (unsigned long)cur_dir, sizeof(cur_dir));
printf("BONJOUR::%lu, SIZE::%lu\n", (unsigned long)bonjour, sizeof(bonjour));

int main(void)
{ t_lbstat *lib;
  unsigned long data;
  data = 0;
  start(lib, &data); }

printf("GG::%s\n", ((t_cdir*)data)->stock);
