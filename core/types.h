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
    hid_device      *device;

    KBS_firmware    firmware;

    bool            connected;

    U8              rows;
    U8              cols;

    U8              layers_count;
} KBS_model;

typedef struct 
{
    F32     x;
    F32     y;

    F32     rx;
    F32     ry;

    U16     code;

    U8      row;
    U8      col;

    U8      width;
    U8      height;
} KBS_key;

typedef struct 
{
    Arena       arena;

    KBS_model   *keyboards;
    size_t      keyboards_count;

    S16         active_model_idx;
} App;



#endif
