
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
  char path[4096];
  char **line;
  int fd;
  if (dir)
  { struct dirent *dp;
    while ((dp = readdir(dir)))
    { if (NCMP("README.md", dp->d_name, 9) == 0 && !dp->d_name[9])
      { openner(coucou, dp->d_name, path);
      //  SéCURITé
        fd = open(path, O_RDONLY);
        while (GNL(fd, line))
        { if ((*line)[0] == '#' && (*line)[1] == ' ')
          { printf("LVL1::%s\n", &((*line)[2])); }
          else if ((*line)[0] == '#' && (*line)[1] == '#' && (*line)[2] == ' ')
          { printf("LVL2::%s\n", &((*line)[3])); }
          DEL((void**)line); }
        close(fd); }
      else if (NCMP("Makefile", dp->d_name, 8) == 0 && !dp->d_name[8])
      { openner(coucou, dp->d_name, path);
      //  SéCURITé
        fd = open(path, O_RDONLY);
        while (GNL(fd, line))
        { int i = 0;
          while ((*line)[i] && (*line)[i] == ' ')
          { i += 1; }
          if (!NCMP(&((*line)[i]), "NAME = ", 7) && LEN(&((*line)[i])) > 7)
          { printf("NAME::%s\n", &((*line)[i + 7])); }
          while ((*line)[i])
          { if (i && ((*line)[i] == ' ' || !(*line)[i]) && (*line)[i - 1] == ':')
            { char *regle = SUB(*line, 0, i - 1);
              if (NCMP(regle, ".PHONY", 6))
              { printf("REGLE::%s\n", regle); }
              else
              { printf("PROTECTION\n"); }
              free(regle); }
            else if ((*line)[i] == ' ')
            { break; }
            i += 1; }
          DEL((void**)line); }
        close(fd); }
      else if (NCMP(".git", dp->d_name, 4) == 0 && !dp->d_name[4])
      { openner(coucou, ".git/logs/HEAD", path);
      //  SéCURITé
        fd = open(path, O_RDONLY);
        GNL(fd, line);
        char **tab = SPL(*line, ' ');
        int len = 0;
        while (tab[len])
        { len += 1; }
        printf("LEN::%d\n", len);
        if (len >= 9)
        { time_t k = (time_t)atoi(tab[5]);
          printf("Nom::%s | Prénom::%s | Date::%s\n", tab[2], tab[3], ctime(&k)); }
        DEL((void**)line);
        close(fd); }}}}

typedef struct s_cdir
{ char stock[4096];
  char actual[4096]; } t_cdir;

void icwd(void *dir)
{ char tmp[4096];
  BZE(tmp, 4096);
  BZE((char *)dir, 4096);
  getcwd(tmp, 4096);
  NCPY((char *)dir, tmp, 4096); }

void start(t_lbstat *lib, void **data)
{ (void)lib;
  static t_cdir tmp[1];
  icwd(&((*tmp).stock));
  *data = &tmp; }

void idle(t_lbstat *lib, void **data)
{ (void)lib;
  t_cdir *tmp = (t_cdir *)(*data);
  icwd(&((*tmp).actual));
  if (NCMP((*tmp).stock, (*tmp).actual, 4096))
  { icwd(&((*tmp).stock));
    check_dir(lib, (*tmp).actual); }}

//printf("2 DATA::%lu, SIZE::%lu\n", (unsigned long)cur_dir, sizeof(cur_dir));
//printf("BONJOUR::%lu, SIZE::%lu\n", (unsigned long)bonjour, sizeof(bonjour));

int main(void)
{ t_lbstat *lib;
  unsigned long data;
  data = 0;
  start(lib, (void**)&data);
  chdir("/Users/jpepin/Ausbildung");
  idle(lib, (void**)&data);
  printf("\n");
  chdir("/Users/jpepin/goinfre/select");
  idle(lib, (void**)&data);
  idle(lib, (void**)&data);
  printf("\n");
  chdir("/Users/jpepin/goinfre/work42/ls");
  idle(lib, (void**)&data);
}

//printf("GG::%s\n", ((t_cdir*)data)->stock);
