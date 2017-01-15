
#include "neko.h"
#include "outils.h"

#include <stdio.h>

// Insère une ligne vide
void push_blank(char *message_bullets, int *mes)
{ message_bullets[*mes] = '\n';
  *mes += 1; }

// Insère une ligne de texte avec une nouvelle ligne
void push_line(char *message_bullets, char *descriptif, int *mes)
{ int len = LEN(descriptif);
  if (*mes + len + 1 >= 1024)
  { return; }
  NCPY(&(message_bullets[*mes]), descriptif, len);
  NCPY(&(message_bullets[*mes + len]), "\n", 1);
  *mes += len + 1; }

// Insère une ligne de texte formattée et avec une nouvelle ligne
void push_sized(char *message_bullets, char *descriptif, int len, int *mes)
{ int k = LEN(descriptif);
  if (k < len)
  { len = k; }
  if (*mes + len + 1 >= 1024)
  { return; }
  NCPY(&(message_bullets[*mes]), descriptif, len);
  NCPY(&(message_bullets[*mes + len]), "\n", 1);
  *mes += len + 1; }

// Insère une ligne de texte
void push_front(char *message_bullets, char *descriptif, int *mes)
{ int len = LEN(descriptif);
  if (*mes + len >= 1024)
  { return; }
  NCPY(&(message_bullets[*mes]), descriptif, len);
  *mes += len; }

// Insère une ligne de texte formattée
void push_part(char *message_bullets, char *descriptif, int len, int *mes)
{ if (*mes + len >= 1024)
  { return; }
  NCPY(&(message_bullets[*mes]), descriptif, len);
  *mes += len; }

// Actualise l'info-bulle avec un nouveau message
void neko_say(t_character *bulle, char *message)
{ if (message)
  { int i = 0;
    while (i < SPEC_CHARACTER_MAX)
    { (bulle[i]).glyph = message[i];
      i += 1; }
    i = 0;
    while (i < SPEC_CHARACTER_MAX)
    { if ((bulle[i]).glyph > 126 || ((bulle[i]).glyph < 32 && (bulle[i]).glyph != 10))
      { (bulle[i]).glyph = 0; }
      i += 1; }}}
