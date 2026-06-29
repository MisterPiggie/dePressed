#ifndef TYPES_H
#define TYPES_H

#include "core/arena.h"
#include "core/num_types.h"
#include <hidapi/hidapi.h>

typedef enum
{
    QMK,
    ZMK,
    VIAL,
} KBS_firmware;

typedef struct
{
    char            *path;
    wchar_t         *product_name;
    KBS_firmware    firmware;

    hid_device      *device;

    U8              layers_count;
    S16             rows;
    S16             cols;
} KBS_model;

typedef struct 
{
    Arena       arena;

    KBS_model   *keyboards;
    size_t      keyboards_count;
} App;



#endif
