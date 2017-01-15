
#include "neko.h"
#include "outils.h"

// Insère une ligne vide
void push_blank(char *message_bullets, int *mes)
{ message_bullets[*mes] = '\n';
  *mes += 16; }

// Insère une ligne de texte formatée
void push_line(char *message_bullets, char *descriptif, int len, int *mes)
{ if (len > 16)
  { len = 16; }
  if (*mes + 17 >= 1024)
  { return; }
  NCPY(&(message_bullets[*mes]), descriptif, len);
  NCPY(&(message_bullets[*mes + len]), "\n", 1);
  *mes += 17; }

// Ajoute du texte dans le buffer sans se soucier du pading
// Restore le padding à la fin
void push_text(char *message_bullets, char *texte, int *mes)
{ int len = LEN(texte);
  int i = 0;
  if (*mes + len + 1 >= 1024)
  { len = 1024 - (*mes + 1); }
  while (i < len)
  { message_bullets[*mes + i] = texte[i];
    i += 1; }
  message_bullets[*mes + i] = '\n';
  while (i % 17 != 0)
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
void adjust(char *message_bullets, int *mes)
{ message_bullets[*mes] = '\n';
  *mes += 17 - (*mes % 17); }

// Actualise l'info-bulle avec un nouveau message
void neko_say(t_character *bulle, char *message)
{ BZE(bulle, SPEC_CHARACTER_MAX);
  if (message)
  { int i = 0;
    while (i < SPEC_CHARACTER_MAX)
    { (bulle[i]).glyph = message[i];
      i += 1; }}}
