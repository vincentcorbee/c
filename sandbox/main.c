#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include "hash-table.h"

int main()
{

  // HashTable *ht = hashTableCreate(200, NULL);

  // hashTableAdd(ht, "name", "John");

  unsigned int Pixel = 0xFFAACC11;

  int r = Pixel >> 24 & 0xFF;

  int g = Pixel >> 16 & 0xFF;

  short b = Pixel >> 8 & 0xFF;

  short a = Pixel & 0xFF;

  printf("r -> %i\n", r);
  printf("g -> %i\n", g);
  printf("b -> %i\n", b);
  printf("a -> %i\n", a);

  int num = 500;

  char *bin = malloc(sizeof(char));

  int i = 0;

  while (num)
  {
    bin[i++] = (num & 0x1) == 1 ? "1" : "0";
    num >>= 1;
  }

  printf("%s\n", bin);

  return 0;
}
