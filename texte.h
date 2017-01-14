
#ifndef TEXTE_H
#define TEXTE_H

void push_blank(int *mes);
void push_line(char *message_bullets, char *descriptif, int len, int *mes);
void push_text(char *message_bullets, char *texte, int *mes);
void push_front(char *message_bullets, char *texte, int *mes);
void adjust(char *message_bullets, int *mes);

#endif
