#ifndef TYPES_H
#define TYPES_H

#include "core/arena.h"

typedef struct 
{
    char    *path;
    wchar_t *product;
} via_keyboard;

typedef struct 
{
   Arena arena;
   via_keyboard *kbs;
   size_t       kbs_count;
} app;


#endif
