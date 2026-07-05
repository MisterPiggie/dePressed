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
    F32     x;
    F32     y;

    F32     rx;
    F32     ry;

    F32     angle;

    U16     *code;

    U8      row;
    U8      col;

    U8      width;
    U8      height;
} KBS_key;

typedef struct
{
    KBS_key *keys;
    U32     key_count;

    U32     *active_layers;
    U32     active_layer_count;
} KBS_layout;

typedef struct
{
    F32     x;
    F32     y;

    F32     rotation_x;
    F32     rotation_y;
    F32     rotation_angle;

    F32     width;
    F32     height;

} KBS_cursor;

typedef struct
{
    char            *path;
    wchar_t         *product_name;
    hid_device      *device;
    KBS_layout      layout;
    

    KBS_firmware    firmware;

    bool            connected;

    U8              rows;
    U8              cols;

    U8              layers_count;
} KBS_model;


typedef struct 
{
    Arena       arena;

    KBS_model   *keyboards;
    size_t      keyboards_count;

    S16         active_model_idx;
} App;



#endif
