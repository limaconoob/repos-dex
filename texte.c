
#include "neko.h"
#include "outils.h"

// Insère une ligne vide
void push_blank(int *mes)
{ *mes += 16; }

// Insère une ligne de texte formatée
void push_line(char *message_bullets, char *descriptif, int len, int *mes)
{ if (len > 16)
  { len = 16; }
  if (*mes + 16 >= 1024)
  { return; }
  NCPY(&(message_bullets[*mes]), descriptif, len);
  *mes += 16; }

// Ajoute du texte dans le buffer sans se soucier du pading
// Restore le padding à la fin
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

// Ajoute du texte dans le buffer sans se soucier du pading
void push_front(char *message_bullets, char *texte, int *mes)
{ int len = LEN(texte);
  int i = 0;
  if (*mes + len >= 1024)
  { len = 1024 - *mes; }
  while (i < len)
  { message_bullets[*mes + i] = texte[i];
    i += 1; }
  *mes += i; }

// Restore le padding
void adjust(int *mes)
{ *mes += 16 - (*mes % 16); }

// Actualise l'info-bulle avec un nouveau message
void neko_say(char *bulle, char *message)
{ BZE(bulle, SPEC_CHARACTER_MAX);
  if (message)
  { int i = 0;
    while (i < SPEC_CHARACTER_MAX && message[i])
    { bulle[i] = message[i]
      i += 1; }}}
