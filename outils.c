
#include <stdlib.h>

int LEN(char *str)
{ int i = 0;
  while (*(str + i))
  { i += 1; }
  return (i); }

char *SUB(char *str, int start, int len)
{ char *ret;
  int i = 0;
  if (!((ret = (char*)malloc(len + 1))))
  { return ((void*)0); }
  while (i < len)
  { ret[i] = *(str + start + i);
    i += 1; }
  ret[i] = 0;
  return (ret); }

void NCPY(void *ptr1, void *ptr2, int size)
{ while (size)
  { ((unsigned char *)ptr1)[size - 1] = ((unsigned char *)ptr2)[size - 1];
    size -= 1; }}

void BZE(void *ptr, int size)
{ while (size)
  { ((unsigned char *)ptr)[size - 1] = 0;
    size -= 1; }}

int NCMP(void *ptr1, void *ptr2, int size)
{ if (size > 0)
  { int i = 0;
    while (i < size && ((unsigned char*)ptr1)[i] == ((unsigned char*)ptr2)[i])
    { i += 1; }
    if (i != size)
    { return (((unsigned char*)ptr1)[i] - ((unsigned char*)ptr2)[i]); }}
  return (0); }
