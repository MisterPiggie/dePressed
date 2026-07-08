#ifndef TYPES_H
#define TYPES_H
#include "core/arena.h"
#include "core/num_types.h"
#include <hidapi/hidapi.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <SDL3/SDL.h>

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
    Arena           arena;
    char            *path;
    wchar_t         *product_name;
    hid_device      *device;
    U8              *lookup;
    bool            *pressed;

    KBS_layout      layout;

    KBS_firmware    firmware;

    bool            connected;

    U8              rows;
    U8              cols;

    U8              layers_count;

} KBS_model;

typedef struct 
{
    pthread_mutex_t mutex;
    U8              active_layer;
    bool            *pressed;
    atomic_bool     running;
} App_shared;

typedef struct 
{
    Arena       arena;

    KBS_model   *keyboards;
    size_t      keyboards_count;

    S16         active_model_idx;
    App_shared  *shared;

    SDL_Window   *window;
    SDL_Renderer *renderer;
} App;



#endif
