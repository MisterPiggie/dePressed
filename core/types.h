#ifndef TYPES_H
#define TYPES_H

#include "core/arena.h"
#include "core/num_types.h"
#include <hidapi/hidapi.h>

typedef struct 
{
    char    *path;
    wchar_t *product;
} via_keyboard;

typedef struct 
{
    hid_device   *device;

    U8      current_layer;
    U8      layers_count;

    S16     rows;
    S16     cols;
} model_keyboard;

typedef struct 
{
    Arena arena;
    via_keyboard *kbs;
    size_t       kbs_count;

    model_keyboard *model;
} app;


#endif
