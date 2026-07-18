#ifndef TYPES_H
#define TYPES_H
#include "core/arena.h"
#include "core/num_types.h"
#include <hidapi/hidapi.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

typedef enum 
{
    SCREEN_SETUP,
    SCREEN_MAIN,
} GUI_state;

typedef enum
{
    QMK,
    ZMK,
    VIAL,
} KBS_firmware;

typedef struct 
{
    SDL_FRect rect;
    SDL_FPoint center;

    SDL_Texture  *pressed_texture;
    SDL_Texture  *idle_texture;

    SDL_Texture  **code_textures;

    SDL_FRect    *code_text_rects;
    SDL_FPoint   *code_text_centers;

    F32       x;
    F32       y;

    F32       rx;
    F32       ry;

    F32       angle;

    U16       *code;

    U8        row;
    U8        col;

    U8        width;
    U8        height;
} KBS_key;

typedef struct
{
    KBS_key *keys;
    U32     key_count;

    F32     layout_width;
    F32     layout_height;

    F32     scale;
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
    char            *product_name;
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
    SDL_FRect   rect;
    SDL_Texture *text_texture;

    bool        is_hovered;
    bool        is_pressed;
} GUI_button;

typedef struct 
{
    SDL_FRect   rect;

    bool        is_hovered;
    U8          idx;
} GUI_dropdown_link;

typedef struct
{
    GUI_dropdown_link *link;

    SDL_FRect   rect;

    SDL_Texture **options_texture;
    SDL_Texture *placeholder_texture;

    S32         win_width;
    S32         win_height;

    bool        is_open;
    bool        is_pressed;
    bool        is_hovered;

    S8          selected_idx;
} GUI_dropdown;

typedef struct 
{
    pthread_mutex_t mutex;
    U32             active_layers;
    bool            *pressed;
    atomic_bool     running;
} App_shared;

typedef struct 
{
    App_shared *shared;
    KBS_model  *model;
} App_thread_arg;

typedef struct 
{
    Arena       arena;

    KBS_model   *keyboards;

    SDL_Texture  *error_texture;
    SDL_FRect    error_rect;
    U64          errored_at_time;


    S16         active_model_idx;
    App_shared  shared;

    SDL_Window   *window;
    SDL_Renderer *renderer;
    TTF_Font     *font;

    GUI_state    current_screen;

    GUI_dropdown dropdown;
    GUI_button   ok_button;
    GUI_button   reload_button;
    GUI_button   exit_button;
    SDL_FRect    drag_handle;

    SDL_Color    bg_color;
    SDL_Color    fg_color;
    SDL_Color    idle_color;
    SDL_Color    pressed_color;
    SDL_Color    hover_color;

    pthread_t    key_listen_thread;

    U8          keyboards_count;

    bool         is_errored;

    bool         running;
} App;



#endif
