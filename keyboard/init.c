#include "keyboard/vial.h"
#include "keyboard/hid.h"
#include "keyboard/init.h"
#include "keyboard/decode.h"
#include <float.h>
#include <SDL3/SDL.h>
#include <math.h>

bool KBS_connect_keyboard(App *app)
{
    KBS_model *model = &app->keyboards[app->active_model_idx];
    model->arena = arena_create(MB(10), KB(1));
    U32 def_size;
    U32 json_size;
    U8 *decompressed_json;

    if (!HID_open_device(model))
    {
        printf("failed open\n");
        return false;
    }
    if (!VIA_confirm_protocol_version(model))
    {
        printf("failed protocol\n");
        return false;
    }
    if (!VIA_get_layers_count(model))
    {
        printf("failed layers count\n");
        return false;
    }
    if (!VIAL_enabled(model))
    {
        printf("failed enabled\n");
        return false;
    }

    model->firmware = VIAL;

    if (!VIAL_get_def_size(model, &def_size))
    {
        printf("failed get size\n");
        return false;
    }

    U8 def_compressed[def_size];

    if (!VIAL_get_def(model, def_compressed, def_size))
    { printf("failed def\n");
        return false;
    }

    decompressed_json = XZ_decode(def_compressed, def_size, &json_size);
    if (!decompressed_json) 
        return false;

    cJSON *json = JSON_parse_buffer(decompressed_json, json_size);
    if (!json)
    {
        printf("JSON\n");
        return false;
    }

    cJSON *matrix = cJSON_GetObjectItemCaseSensitive(json, "matrix");
    if(!cJSON_IsObject(matrix))
    {
        printf("matrix\n");
        return false;
    }
    
    cJSON *rows = cJSON_GetObjectItemCaseSensitive(matrix, "rows");
    if (!cJSON_IsNumber(rows))
    {
        printf("rows\n");
        return false;
    }

    cJSON *cols = cJSON_GetObjectItemCaseSensitive(matrix, "cols");
    if (!cJSON_IsNumber(cols))
    {
        printf("cols\n");
        return false;
    }

    model->rows = rows->valueint;
    model->cols = cols->valueint;
    model->layout.key_count = model->rows * model->cols;

    cJSON *json_layouts = cJSON_GetObjectItemCaseSensitive(json, "layouts");
    if (!cJSON_IsObject(json_layouts))
    {
        printf("json layouts\n");
        return false;
    }

    cJSON *json_keymap = cJSON_GetObjectItemCaseSensitive(json_layouts, "keymap");
    if (!cJSON_IsArray(json_keymap))
    {
        printf("json keymap\n");
        return false;
    }

    model->layout.keys = arena_push_array(&model->arena, KBS_key, model->layout.key_count);

    KBS_cursor cursor = {0};
    cursor.y = -1;
    cursor.width = 1;
    cursor.height = 1;

    cJSON *row;
    U8 key_idx = 0;

    cJSON_ArrayForEach(row, json_keymap)
    {
        if (!cJSON_IsArray(row))
            continue;


        cursor.y += 1;
        cursor.x = cursor.rotation_x;

        cJSON *item;
        cJSON_ArrayForEach(item, row)
        {
            if (cJSON_IsObject(item))
                apply_offset(&cursor, item);
            else if (cJSON_IsString(item))
            {
                if (is_multiline_label(item->valuestring))
                {
                    cursor.x += cursor.width;
                    cursor.width = 1;
                    cursor.height = 1;
                    continue; 
                }
                KBS_key *key = &model->layout.keys[key_idx];
                key->x = cursor.x;
                key->y = cursor.y;
                key->width = cursor.width;
                key->height = cursor.height;
                key->angle = cursor.rotation_angle;
                key->rx = cursor.rotation_x;
                key->ry = cursor.rotation_y;

                parse_row_col(item->valuestring, &key->row, &key->col);


                cursor.x += cursor.width;
                cursor.width = 1;
                cursor.height = 1;
                key_idx++;
            }
        }
    }

    cJSON_Delete(json);


    U32 keymap_size = model->layers_count * model->rows * model->cols * 2;
    U8 keymap_buf[keymap_size];

    if(!VIAL_get_keymap(model, keymap_buf, keymap_size))
    {
        printf("Keymap fail\n");
        return false;
    }

    model->lookup = arena_push_array(&model->arena, U8, model->layout.key_count);
    model->pressed = arena_push_array_zero(&model->arena, bool, model->layout.key_count);
    app->shared.pressed = model->pressed;

    for (int i = 0; i < model->layout.key_count; i++)
    {
        KBS_key *key = &model->layout.keys[i];
        key->code = arena_push_array(&model->arena, U16, model->layers_count);


        for (int j = 0; j < model->layers_count; j++)
        {
            U32 slot_idx = key->row * model->cols + key->col + (model->rows * model->cols) * j;
            U32 byte_idx = slot_idx * 2;
            key->code[j] = keymap_buf[byte_idx] | (keymap_buf[byte_idx + 1] << 8);
        }

        U8 slot = key->row * model->cols + key->col;
        model->lookup[slot] = i;
    }

    KBS_get_bounds(app);

    if (!KBS_start_key_listener(app))
        assert(1);

    return true;
}
static void apply_offset(KBS_cursor *cursor, cJSON *item)
{
    cJSON *v;
    if ((v = cJSON_GetObjectItem(item, "r")))
        cursor->rotation_angle = v->valuedouble;

    if ((v = cJSON_GetObjectItem(item, "rx")))
    {
        cursor->rotation_x = v->valuedouble;
        cursor->x = cursor->rotation_x;
    }
    if ((v = cJSON_GetObjectItem(item, "ry")))
    {
        cursor->rotation_y = v->valuedouble;
        cursor->y = cursor->rotation_y;
    }

    if ((v = cJSON_GetObjectItem(item, "x")))
        cursor->x += v->valuedouble;
    if ((v = cJSON_GetObjectItem(item, "y")))
        cursor->y += v->valuedouble;
    if ((v = cJSON_GetObjectItem(item, "w")))
        cursor->width = v->valuedouble;
    if ((v = cJSON_GetObjectItem(item, "h")))
        cursor->height = v->valuedouble;
}

static void parse_row_col(const char *label, U8 *row, U8 *col)
{
    char buf[32];
    U32 temp_row = 0;
    U32 temp_col = 0;
    const char *nl = strchr(label, '\n');
    size_t len = nl ? (size_t)(nl - label) : strlen(label);
    if (len >= sizeof(buf)) 
        len = sizeof(buf) - 1;
    memcpy(buf, label, len);

    buf[len] = '\0';
    sscanf(buf, "%d,%d", &temp_row, &temp_col);
    *row = (U8)temp_row;
    *col = (U8)temp_col;
}

static void KBS_get_bounds(App *app)
{
    KBS_model *model = &app->keyboards[app->active_model_idx];
    F32 min_x = FLT_MAX, max_x = -FLT_MAX;
    F32 min_y = FLT_MAX, max_y = -FLT_MAX;

    for (int i = 0; i < model->layout.key_count; i++)
    {
        KBS_key *key = &model->layout.keys[i];

        min_x = fminf(min_x, key->x);
        min_y = fminf(min_y, key->y);
        max_x = fmaxf(max_x, key->x + key->width);
        max_y = fmaxf(max_y, key->y + key->height);
    }
    
    model->layout.layout_width = max_x - min_x + MARGIN * 2;
    model->layout.layout_height = max_y - min_y + MARGIN * 2;

    S32 window_width, window_height;

    SDL_GetWindowSize(app->window, &window_width, &window_height);

    model->layout.scale = fminf(window_width / model->layout.layout_width, 
                                window_height / model->layout.layout_height);

    for (int i = 0; i < model->layout.key_count; i++)
    {
        KBS_key *key = &model->layout.keys[i];

        F32 px_x = (key->x - min_x + MARGIN) * model->layout.scale;
        F32 px_y = (key->y - min_y + MARGIN) * model->layout.scale;
        F32 px_w = key->width * model->layout.scale;
        F32 px_h = key->height * model->layout.scale;

        F32 px_pad = BEETWEEN_KEY_PADDING * model->layout.scale;

        key->rect = (SDL_FRect)
        {
            px_x + px_pad / 2.0f,
            px_y + px_pad / 2.0f,
            px_w - px_pad, 
            px_h - px_pad,
        };

        key->idle_texture = SDL_CreateTexture(app->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, key->rect.w, key->rect.h);
        key->pressed_texture = SDL_CreateTexture(app->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, key->rect.w, key->rect.h);

        SDL_SetRenderTarget(app->renderer, key->idle_texture);
        SDL_SetRenderDrawColor(app->renderer, app->idle_color.r, app->idle_color.g, app->idle_color.b, 255);
        SDL_RenderClear(app->renderer);

        SDL_RenderFillRect(app->renderer, NULL);
        SDL_SetRenderTarget(app->renderer, NULL);

        SDL_SetRenderTarget(app->renderer, key->pressed_texture);
        SDL_SetRenderDrawColor(app->renderer, app->pressed_color.r, app->pressed_color.g, app->pressed_color.b, 255);
        SDL_RenderClear(app->renderer);

        SDL_RenderFillRect(app->renderer, NULL);
        SDL_SetRenderTarget(app->renderer, NULL);

        SDL_FPoint center; 

        F32 pivot_abs_x = (key->rx - min_x + MARGIN) * model->layout.scale + px_pad / 2.0f;
        F32 pivot_abs_y = (key->ry - min_y + MARGIN) * model->layout.scale + px_pad / 2.0f;
        key->center = (SDL_FPoint){
            pivot_abs_x - key->rect.x,
            pivot_abs_y - key->rect.y
        };

        printf("Keys data:\n X: %f Y: %f RX: %f RY: %f Angle: %f\n", key->x, key->y, key->rx, key->ry, key->angle);


    }

    
}

static bool is_multiline_label(const char *label)
{
    return strchr(label, '\n') != NULL;
}

void *KBS_key_listener_thread(void *arg)
{
    App_thread_arg *args = (App_thread_arg *)arg;
    listen_for_keypresses(args->model, args->shared);
    return NULL;
}

bool KBS_start_key_listener(App *app)
{
    if (pthread_mutex_init(&app->shared.mutex, NULL) != 0) return false;
    atomic_init(&app->shared.running, true);
    app->shared.active_layer = 0;

    KBS_model *model = &app->keyboards[app->active_model_idx];

    App_thread_arg *args = arena_push_struct(&app->arena, App_thread_arg);
    args->model = model;
    args->shared = &app->shared;

    if (pthread_create(&app->key_listen_thread, NULL, KBS_key_listener_thread, args) != 0)
    {
        app->shared.pressed = NULL;
        pthread_mutex_destroy(&app->shared.mutex);
        return false;
    }
    return true;
}
