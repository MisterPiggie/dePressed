#include <hidapi/hidapi.h>
#include <SDL3/SDL.h>
#include "keyboard/hid.h"
#include "GUI/font.h"
#include "GUI/button.h"

bool HID_get_suitable_keyboards(App *app)
{
    struct hid_device_info *device_linked_list = hid_enumerate(0, 0);
    if (device_linked_list == NULL)
        return false;
    struct hid_device_info *node = device_linked_list;

    while (node != NULL)
    {
        if (node->usage_page == HID_USAGE_PAGE && node->usage == 0x61) 
            app->keyboards_count++;
        node = node->next;
    }
    
    if (app->keyboards_count == 0)
    {
        app->dropdown.selected_idx = -1;
        return false;
    }

    app->keyboards = arena_push_array(&app->arena, KBS_model, app->keyboards_count);

    node = device_linked_list;
    int i = 0;

    while (node != NULL)
    {
        if (node->usage_page == HID_USAGE_PAGE && node->usage == 0x61)
        {
            HID_device_info_to_KBS_model(&app->arena, node, &app->keyboards[i]);
            if (i < app->keyboards_count)
                i++;
        }
        node = node->next;
    }
    hid_free_enumeration(device_linked_list);

    app->dropdown.selected_idx = 0;

    app->dropdown.options_texture = arena_push_array(&app->arena, SDL_Texture *, app->keyboards_count);
    app->dropdown.link = arena_push_array(&app->arena, GUI_dropdown_link, app->keyboards_count);

    for (int i = 0; i < app->keyboards_count; i++)
    {
        app->dropdown.options_texture[i] = GUI_make_font_texture(app->font, app->renderer, app->keyboards[i].product_name, app->fg_color);
        app->dropdown.link[i].rect = (SDL_FRect)
        {
            .x = app->dropdown.win_width * X_FRAC_DROP,
            .y = app->dropdown.win_height * (Y_FRAC_DROP * (2 + i)),
            .w = app->dropdown.win_width * W_FRAC_DROP,
            .h = app->dropdown.win_height * H_FRAC_DROP,
        };
        app->dropdown.link[i].idx = i;
        app->dropdown.link[i].is_hovered = false;
    }

    return true;
}

void HID_device_info_to_KBS_model(Arena *arena, struct hid_device_info *info, KBS_model *model)
{
    model->path = arena_push_str(arena, info->path);
    char *utf8_str = SDL_iconv_wchar_utf8(info->product_string); // THIS IS SUCH A HACK I LOVE IT
    model->product_name = arena_push_str(arena, utf8_str);
    SDL_free(utf8_str);

    model->rows = 0;
    model->cols = 0;
    model->layers_count = 0;
    model->connected = false;

    model->firmware = QMK;
}

bool HID_open_device(KBS_model *model)
{
    model->device = hid_open_path(model->path);
    if (model->device == NULL)
    {
        model->connected = false;
        return model->connected;
    }

    model->connected = true;

    return model->connected;
}

void listen_for_keypresses(KBS_model *model, App_shared *shared)
{
    U8 buf[RAW_HID_PACKET_SIZE];

    while (atomic_load(&shared->running))
    {
        int res = hid_read_timeout(model->device, buf, RAW_HID_PACKET_SIZE, 100);

        if (res > 0) 
        {
            if (buf[0] == ID_CUSTOM_KEY_EVENT)
            {
                U8 row = buf[1], col = buf[2], pressed = buf[3];

                pthread_mutex_lock(&shared->mutex);
                model->pressed[model->lookup[row * model->cols + col]] = (pressed == 1);
                pthread_mutex_unlock(&shared->mutex);

            } else if (buf[0] == ID_CUSTOM_LAYER_EVENT)
            {
                pthread_mutex_lock(&shared->mutex);
                shared->active_layer = buf[1];
                pthread_mutex_unlock(&shared->mutex);
            }
        }
    }
}
