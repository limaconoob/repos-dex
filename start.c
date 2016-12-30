
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

void push_blank(int *mes)
{ *mes += 16; }

// Ajoute une ligne avec le pading (16 caractères max)
void push_line(char *message_bullets, char *descriptif, int len, int *mes)
{ if (len > 16)
  { len = 16; }
  if (*mes + 16 >= 1024)
  { return; }
  NCPY(&(message_bullets[*mes]), descriptif, len);
  *mes += 16; }

// Ajoute du texte dans le buffer sans se soucier du pading
void push_text(char *message_bullets, char *texte, int *mes)
{ int len = LEN(texte);
  int i = 0;
  if (*mes + len >= 1024)
  { len = 1024 - *mes; }
  while (i < len)
  { message_bullets[*mes + i] = texte[i];
    i += 1; }
  while (i % 16 != 0)
  { i += 1; }
  *mes += i; }

void push_front(char *message_bullets, char *texte, int *mes)
{ int len = LEN(texte);
  int i = 0;
  if (*mes + len >= 1024)
  { len = 1024 - *mes; }
  while (i < len)
  { message_bullets[*mes + i] = texte[i];
    i += 1; }
  *mes += i; }

void adjust(int *mes)
{ *mes += 16 - (*mes % 16); }

void check_dir(t_lbstat *lib, char *coucou, void *message_bullets)
{ DIR *dir = opendir(coucou);
  char path[4096];
  int mes = 0;
  char **line;
  int len = 0;
  int index = 0;
  int fd;
  int name = 0;
  BZE(message_bullets, 1024);
  push_line((char *)message_bullets, "Vous etes dans", 14, &mes);
  push_line((char *)message_bullets, "le dossier:", 11, &mes);
  push_text((char *)message_bullets, coucou, &mes);
  push_blank(&mes);
  if (dir)
  { struct dirent *dp;
    while ((dp = readdir(dir)))
    { if (NCMP("README.md", dp->d_name, 9) == 0 && !dp->d_name[9])
      { openner(coucou, dp->d_name, path);
      //  SéCURITé
        fd = open(path, O_RDONLY);
        push_line((char *)message_bullets, "J'ai trouve' un", 15, &mes);
        push_line((char *)message_bullets, "readme.md:", 10, &mes);
        while (GNL(fd, line))
        { if ((*line)[0] == '#' && (*line)[1] == ' ')
          { push_text((char *)message_bullets, *line, &mes); }
          else if ((*line)[0] == '#' && (*line)[1] == '#' && (*line)[2] == ' ')
          { push_front((char *)message_bullets, "- ", &mes);
            push_text((char *)message_bullets, &((*line)[3]), &mes); }
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
          { push_line((char *)message_bullets, "Nom du projet:", 14, &mes);
            push_line((char *)message_bullets, &((*line)[i + 7]), LEN(&((*line)[i + 7])), &mes);
            push_blank(&mes);
            push_line((char *)message_bullets, "Voici les regles", 16, &mes);
            push_line((char *)message_bullets, "du Makefile:", 12, &mes); }
          while ((*line)[i])
           { if (i && (((*line)[i] == ' ' && (*line)[i - 1] == ':') || (!(*line)[i + 1] && (*line)[i++] == ':')))
            { char *regle = SUB(*line, 0, i - 1);
              if (*regle != '.')
              { push_line((char *)message_bullets, regle, LEN(regle), &mes); }
              free(regle); }
            else if ((*line)[i] == ' ')
            { break; }
            i += 1; }
          DEL((void**)line); }
          push_blank(&mes);
        close(fd); }

      else if (NCMP("Cargo.toml", dp->d_name, 10) == 0 && !dp->d_name[10])
      { openner(coucou, dp->d_name, path);
      //  SéCURITé
        fd = open(path, O_RDONLY);
        char auteur = 0;
        while (GNL(fd, line))
        { int i = 0;
          while ((*line)[i] && ((*line)[i] == ' ' || (*line)[i] == '\t'))
          { i += 1; }
          if (!name && !NCMP(&((*line)[i]), "name = \"", 8) && LEN(&((*line)[i])) > 8)
          { push_line((char *)message_bullets, "Nom du projet:", 14, &mes);
            push_line((char *)message_bullets, &((*line)[i + 8]), LEN(&((*line)[i + 8])) - 1, &mes);
            name = 1; }
          else if (!NCMP(&((*line)[i]), "version = \"", 11) && LEN(&((*line)[i])) > 11)
          { push_line((char *)message_bullets, "Version:", 8, &mes);
            push_line((char *)message_bullets, &((*line)[i + 11]), LEN(&((*line)[i + 11])) - 1, &mes);
            push_blank(&mes); }
          else if (auteur && **line != ']' && **line != '[')
          { i += 1;
            int q = 0;
            while ((*line)[i + q] && (*line)[i + q + 1] != '<')
            { q += 1; }
            push_line((char *)message_bullets, &((*line)[i]), q, &mes); }
          else if (auteur)
          { auteur = 0; }
          else if (!NCMP(&((*line)[i]), "authors = [", 11))
          { push_line((char *)message_bullets, "Ce projet a ete", 15, &mes);
            push_line((char *)message_bullets, "realise par:", 12, &mes);
            auteur = 1; }
          DEL((void**)line); }
        push_blank(&mes);
        push_line((char *)message_bullets, "Je remarque que", 15, &mes);
        push_line((char *)message_bullets, "ce projet", 9, &mes);
        push_line((char *)message_bullets, "utilise cargo:", 14, &mes);
        push_line((char *)message_bullets, "> cargo run", 11, &mes);
        push_line((char *)message_bullets, "> cargo test", 12, &mes);
        push_line((char *)message_bullets, "> cargo --help", 14, &mes);
        push_blank(&mes);
        close(fd); }
      else if (NCMP(".git", dp->d_name, 4) == 0 && !dp->d_name[4])
      { openner(coucou, ".git/logs/HEAD", path);
      //  SéCURITé
        char *tmp;
        tmp = NULL;
        fd = open(path, O_RDONLY);
        push_line((char *)message_bullets, "Ce projet a", 11, &mes);
        push_line((char *)message_bullets, "un repo git!", 12, &mes);
        push_blank(&mes);
        while (GNL(fd, line))
        { if (tmp)
          { DEL((void**)&tmp); }
          tmp = DUP(*line);
          DEL((void**)line); }
        *line = tmp;
        char **tab = SPL(*line, ' ');
        int commit = 0;
        while (tab[commit] && *(tab[commit]) != '+')
        { commit += 1; }
        if (tab[commit] && !(NCMP(tab[commit], "+0100\tcommit:", 13)))
        { push_line((char *)message_bullets, "Dernier commit:", 15, &mes);
          commit += 1;
          while (tab[commit])
          { push_front((char *)message_bullets, tab[commit], &mes);
            push_front((char *)message_bullets, " ", &mes);
            commit += 1; }
          adjust(&mes);
          push_line((char *)message_bullets, "push par:", 9, &mes);
          push_line((char *)message_bullets, tab[2], LEN(tab[2]), &mes);
          push_line((char *)message_bullets, "depuis le:", 10, &mes);
          time_t k = (time_t)atoi(tab[5]);
          push_line((char *)message_bullets, ctime(&k), 10, &mes);
          push_blank(&mes); }
        DEL((void**)line);
        close(fd);
        openner(coucou, ".git/refs/heads", path);
      //  SéCURITé
        DIR *git = opendir(path);
        if (git)
        { struct dirent *db;
          push_line((char *)message_bullets, "Voici toutes les", 16, &mes);
          push_line((char *)message_bullets, "branch locales:", 15, &mes);
          while ((db = readdir(git)))
          { if (*(db->d_name) != '.')
            { push_line((char *)message_bullets, db->d_name, LEN(db->d_name), &mes); }}
          push_blank(&mes);
          (void)closedir(git); }}}
    (void)closedir(dir); }}

typedef struct s_cdir
{ char stock[4096];
  char actual[4096];
  char message_bullets[1024];
  time_t current; } t_cdir;

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
  BZE((*tmp).message_bullets, 1024);
  (*tmp).current = 0;
  *data = &tmp; }

void idle(t_lbstat *lib, void **data)
{ (void)lib;
  t_cdir *tmp = (t_cdir *)(*data);
  icwd(&((*tmp).actual));
  if (NCMP((*tmp).stock, (*tmp).actual, 4096))
  { icwd(&((*tmp).stock));
    check_dir(lib, (*tmp).actual, &((*tmp).message_bullets));
    
    int i = 0;
    printf("BULLET::\n");
    while (i < 1024)
    { printf("%c", ((char *)((*tmp).message_bullets))[i]);
      if (!((char *)((*tmp).message_bullets))[i])
      { printf("."); }
      if ((i + 1) % 16 == 0)
      { printf("\n"); }
      i += 1; }
    printf("\n");
  }
/*  else if ((*tmp).current)
  {}*/ }

int main(void)
{ t_lbstat *lib;
  unsigned long data;
  data = 0;
  start(lib, (void**)&data);
  chdir("/Users/jpepin/Ausbildung");
  idle(lib, (void**)&data);
  printf("\n");
  chdir("/Users/jpepin/goinfre/work42/ls");
  idle(lib, (void**)&data);
  printf("\n");
  chdir("/Users/jpepin/goinfre/nTerm");
  idle(lib, (void**)&data);
  printf("\n");
  chdir("/Users/jpepin/goinfre/top");
  idle(lib, (void**)&data);
}
