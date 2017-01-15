
#ifndef TEXTE_H
#define TEXTE_H

void push_blank(char *message_bullets, int *mes);
void push_line(char *message_bullets, char *descriptif, int *mes);
void push_sized(char *message_bullets, char *descriptif, int len, int *mes);
void push_front(char *message_bullets, char *texte, int *mes);
void push_part(char *message_bullets, char *descriptif, int len, int *mes);
void neko_say(t_character *bulle, char *message);

#endif
