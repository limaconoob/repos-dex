
#include "dex.h"
#include "neko.h"
#include "outils.h"
#include "texte.h"
#include <dirent.h>
#include <fcntl.h>
#include <libproc.h>
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

typedef enum e_flags
{ Cargo,
  Makefile,
  Git,
  ReadMe, } t_flags;

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

void check_dir(t_lbstat *lib, char *coucou, void *message_bullets)
{ DIR *dir = opendir(coucou);
  char path[4096];
  int mes = 0;
  char *line[1];
  int len = 0;
  int index = 0;
  int fd;
  int name = 0;
  int version = 0;
  char flags[4];
  BZE(flags, 4);
  BZE(message_bullets, 1024);
  push_line((char *)message_bullets, "Vous etes dans le dossier:", &mes);
  push_line((char *)message_bullets, coucou, &mes);
  push_blank((char *)message_bullets, &mes);
  if (dir)
  { struct dirent *dp;
    while ((dp = readdir(dir)))
    { if (NCMP("README.md", dp->d_name, 9) == 0 && !dp->d_name[9])
      { flags[ReadMe] = 1; }
      else if (NCMP("Makefile", dp->d_name, 8) == 0 && !dp->d_name[8])
      { flags[Makefile] = 1; }
      else if (NCMP("Cargo.toml", dp->d_name, 10) == 0 && !dp->d_name[10])
      { flags[Cargo] = 1; }
      else if (NCMP(".git", dp->d_name, 4) == 0 && !dp->d_name[4])
      { flags[Git] = 1; }}
    (void)closedir(dir);

    if (flags[Cargo])
    { openner(coucou, "Cargo.toml", path);
      if (!access(path, R_OK))
      { fd = open(path, O_RDONLY);
        char auteur = 0;
        while (GNL(fd, line))
        { int i = 0;
          while ((*line)[i] && ((*line)[i] == ' ' || (*line)[i] == '\t'))
          { i += 1; }
          if (!name && !NCMP(&((*line)[i]), "name = \"", 8) && LEN(&((*line)[i])) > 8)
          { push_front((char *)message_bullets, "Le nom de ce projet est ", &mes);
            push_sized((char *)message_bullets, &((*line)[i + 8]), LEN(&((*line)[i + 8])) - 1, &mes);
            name = 1; }
          else if (!NCMP(&((*line)[i]), "version = \"", 11) && LEN(&((*line)[i])) > 11 && !version)
          { push_front((char *)message_bullets, "et il en est a la version ", &mes);
            push_sized((char *)message_bullets, &((*line)[i + 11]), LEN(&((*line)[i + 11])) - 1, &mes);
            push_blank((char *)message_bullets, &mes);
            version = 1; }
          else if (auteur && **line != ']' && **line != '[')
          { i += 1;
            int q = 0;
            while ((*line)[i + q] && (*line)[i + q + 1] != '<')
            { q += 1; }
            if (auteur > 1)
            { push_front((char *)message_bullets, " & ", &mes);
              push_part((char *)message_bullets, &((*line)[i]), q, &mes); }
            else
            { push_part((char *)message_bullets, &((*line)[i]), q, &mes); }
            auteur += 1; }
          else if (auteur)
          { push_blank((char *)message_bullets, &mes);
            auteur = 0; }
          else if (!NCMP(&((*line)[i]), "author", 6))
          { push_front((char *)message_bullets, "Ce projet a ete realise par ", &mes);
            auteur = 1; }
          DEL((void**)line); }
        push_line((char *)message_bullets, "Je remarque que ce projet utilise cargo.", &mes);
        push_line((char *)message_bullets, "Si vous ne l'avez pas, essayez ca:", &mes);
        push_line((char *)message_bullets, "$> curl https://sh.rustup.rs -sSf | sh", &mes);
        push_line((char *)message_bullets, "$> rustup default nightly", &mes);
        push_line((char *)message_bullets, "$> cargo run", &mes);
        push_blank((char *)message_bullets, &mes);
        close(fd); }}

    else if (flags[Makefile])
    { openner(coucou, "Makefile", path);
      if (!access(path, R_OK))
      { fd = open(path, O_RDONLY);
        while (GNL(fd, line))
        { int i = 0;
          while ((*line)[i] && (*line)[i] == ' ')
          { i += 1; }
          if ((!NCMP(&((*line)[i]), "NAME = ", 7) || !NCMP(&((*line)[i]), "NAME := ", 8)) && LEN(&((*line)[i])) > 7)
          { push_front((char *)message_bullets, "Le nom de ce projet est ", &mes);
            push_sized((char *)message_bullets, &((*line)[i + 7]), LEN(&((*line)[i + 7])), &mes);
            push_blank((char *)message_bullets, &mes);
            push_line((char *)message_bullets, "Voici les regles de son Makefile:", &mes); }
          while ((*line)[i])
           { if (i && (((*line)[i] == ' ' && (*line)[i - 1] == ':') || (!(*line)[i + 1] && (*line)[i++] == ':')))
            { char *regle = SUB(*line, 0, i - 1);
              if (*regle != '.')
              { push_line((char *)message_bullets, regle, &mes); }
              free(regle); }
            else if ((*line)[i] == ' ')
            { break; }
            i += 1; }
          DEL((void**)line); }
          push_blank((char *)message_bullets, &mes);
        close(fd); }}

    if (flags[Git])
    { openner(coucou, ".git/logs/HEAD", path);
      if (!access(path, R_OK))
      { char *tmp;
        tmp = NULL;
        fd = open(path, O_RDONLY);
        push_line((char *)message_bullets, "Ce projet a un repo git!", &mes);
        push_blank((char *)message_bullets, &mes);
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
        { push_line((char *)message_bullets, "Nom du dernier commit:", &mes);
          commit += 1;
          while (tab[commit])
          { push_front((char *)message_bullets, tab[commit], &mes);
            push_front((char *)message_bullets, " ", &mes);
            commit += 1; }
          push_line((char *)message_bullets, "push par:", &mes);
          push_line((char *)message_bullets, tab[2], &mes);
          push_line((char *)message_bullets, "au jour du:", &mes);
          time_t k = (time_t)atoi(tab[5]);
          push_sized((char *)message_bullets, ctime(&k), 10, &mes);
          push_blank((char *)message_bullets, &mes); }
        DEL((void**)line);
        close(fd);
        openner(coucou, ".git/refs/heads", path);
        if (!access(path, R_OK))
        { DIR *git = opendir(path);
          if (git)
          { struct dirent *db;
            push_line((char *)message_bullets, "Voici toutes les branches locales:", &mes);
            while ((db = readdir(git)))
            { if (*(db->d_name) != '.')
              { push_line((char *)message_bullets, db->d_name, &mes); }}
            push_blank((char *)message_bullets, &mes);
            (void)closedir(git); }}}}

    if (flags[ReadMe])
    { openner(coucou, "README.md", path);
      if (!access(path, R_OK))
      { fd = open(path, O_RDONLY);
        push_line((char *)message_bullets, "J'ai aussi trouve un Readme.md!", &mes);
        while (GNL(fd, line))
        { if ((*line)[0] == '#' && (*line)[1] == ' ')
          { push_sized((char *)message_bullets, *line, 50, &mes); }
          else if ((*line)[0] == '#' && (*line)[1] == '#' && (*line)[2] == ' ')
          { push_front((char *)message_bullets, "- ", &mes);
            push_sized((char *)message_bullets, &((*line)[3]), 48, &mes); }
          DEL((void**)line); }
        close(fd); }}}
  push_front((char *)message_bullets, "\x07", &mes); }

void icwd(void *dir)
{ BZE((char *)dir, 4096);
  char tmp[MAXPATHLEN];
  BZE(tmp, MAXPATHLEN);
  struct proc_vnodepathinfo vpi;
  pid_t pids[1];
  proc_listchildpids(getpid(), pids, sizeof(pids));
  proc_pidinfo(pids[0], PROC_PIDVNODEPATHINFO, 0, &vpi, sizeof(vpi));
  NCPY(tmp, vpi.pvi_cdir.vip_path, MAXPATHLEN);
  NCPY((char *)dir, tmp, MAXPATHLEN); }

char not_empty(void *message_bullets, int len)
{ while (len)
  { if (((char *)message_bullets)[len])
    { return (1); }
    len -= 1; }
  return ((*((char **)message_bullets))[len] ? 1 : 0); }

void idle(t_lbstat *lib, void **data)
{ (void)lib;
  static int start = 0;
  t_cdir *tmp = (t_cdir *)(*data);
  icwd(&((*tmp).actual));
  if (start && NCMP((*tmp).stock, (*tmp).actual, 4096))
  { icwd(&((*tmp).stock));
    (*tmp).collider = 0;
    check_dir(lib, (*tmp).actual, &((*tmp).message_bullets));
    (*lib).neko.position.cardinal = MiddleLeft;
    (*lib).infobulle.cardinal = Right;
    neko_say((*lib).infobulle.message, (*tmp).message_bullets); }
  start = 1; }
