#ifndef OUTILS_H
#define OUTILS_H
  int LEN(char *str);
  void BZE(void *ptr, int size);
  int GNL(char **line, int fd);
  int NCMP(void *ptr1, void *ptr2, int size);
  char *SUB(char *str, int start, int len);
  void NCPY(void *ptr1, void *ptr2, int size);
#endif
