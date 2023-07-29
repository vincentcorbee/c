#ifndef arguments_H
#define arguments_H

/* Set EXTERN macro: */
#ifdef arguments_IMPORT
#define EXTERN
#else
#define EXTERN extern
#endif

/* Type declarations */

typedef struct Arguments
{
  char *file;
} Arguments;

/* Function prototypes */

Arguments *getArguments(int argc, char *argv[]);

#undef arguments_IMPORT
#undef EXTERN
#endif