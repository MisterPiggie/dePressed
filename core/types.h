#ifndef TYPES_H
#define TYPES_H

#include "core/arena.h"
#include "core/num_types.h"
#include <hidapi/hidapi.h>
#include <stdbool.h>

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
    bool            connected;

    U8              layers_count;
    S16             rows;
    S16             cols;

} KBS_model;

typedef struct 
{
    U8      x;
    U8      y;

    U16     code;
     
} KBS_key;

typedef struct 
{
    Arena       arena;

    KBS_model   *keyboards;
    size_t      keyboards_count;

    S16         active_model_idx;
} App;



#endif
