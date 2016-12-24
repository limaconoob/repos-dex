
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <unistd.h>
#include "neko.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

// Descritpion du module repos-dex:
//  start : initialise le dossier courant
//   idle : Si on change de dossier courant et qu'il y a présence
//          d'un `.git` &| d'un `README.mb` &| d'un `Makefile`,
//          ça va extraire des infos de ces fichiers :
//            - Git: status, branches, nom du répo, nom du propriétaire
//            - Readme: Affiche les noms des chapitres `#`
//            - Makefile: Affiche les règles du Makefile et $NAME

void BZE(void *ptr, int size)
{ if (size > 0)
  { int i = 0;
    while (i < size)
    { ((unsigned char*)ptr)[i] = 0;
      i += 1; }}}

int CMP(void *ptr1, void *ptr2, int size)
{ if (size > 0)
  { int i = 0;
    while (i < size && ((unsigned char*)ptr1)[i] == ((unsigned char*)ptr2)[i])
    { i += 1; }
    if (i != size)
    { return (((unsigned char*)ptr1)[i] - ((unsigned char*)ptr2)[i]); }}
  return (0); }

void print_path(char *coucou)
{ printf("PATH::");
  int i = 0;
  while (coucou[i])
  { printf("%c", coucou[i]);
    i += 1; }}

char *openner(char *coucou, char *file)
{ char concat[8192];
  int i = 0;
  while (*coucou)
  { concat[i] = *coucou;
    coucou += 1;
    i += 1; }
  while (*file)
  { concat[i] = *file;
    file += 1;
    i += 1; }
  return (concat); }

void check_dir(t_lbstat *lib, char *coucou)
{ DIR *dir = opendir(coucou);
  if (dir)
  { struct dirent *dp;
    while ((dp = readdir(dir)))
    { if (CMP((std::string)"README.mb", dp->d_name, 9) == 0)
      { std::ifstream file;
        char *path = openner(coucou, dp->d_name);
        file.open(path);
        string line;
        while (getline(file,line))
          std::cout << line << '\n';
        file.close(); }
      else if (CMP((std::string)"Makefile", dp->d_name, 8) == 0)
      {}
}}}

class path 
{ char *path;
  public:
    void init(char*); };

class the : public path {};

void start(t_lbstat *lib, void *cur_dir)
{ getcwd((char*)cur_dir, 4096); }


void idle(t_lbstat *lib, void *cur_dir)
{
}

int main(void)
{ t_lbstat *lib;
  std::string input = "";
  char coucou[4096];
  char actual[4096];
  BZE(coucou, 4096);
  BZE(actual, 4096);
  start(lib, coucou);
  while (true)
  { start(lib, actual);
    getline(std::cin, input);
    if (input[0] == 's')
    { chdir("/Users/jpepin/Ausbildung"); }
    else if (input[0] == 'q')
    { break ; }
    if (CMP(coucou, actual, 4096))
    { BZE(coucou, 4096);
      start(lib, coucou);
      print_path(coucou); }}}
