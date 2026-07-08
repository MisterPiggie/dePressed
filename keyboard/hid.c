#include <hidapi/hidapi.h>
#include "keyboard/hid.h"

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
        return false;

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

    return true;
}

void HID_device_info_to_KBS_model(Arena *arena, struct hid_device_info *info, KBS_model *model)
{
    model->path = arena_push_str(arena, info->path);
    model->product_name = arena_push_wstr(arena, info->product_string);

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

    while (1)
    {
        int res = hid_read_timeout(model->device, buf, RAW_HID_PACKET_SIZE, 100);

        if (res > 0) 
        {
            if (buf[0] == ID_CUSTOM_KEY_EVENT)
            {
                U8 row = buf[1], col = buf[2], pressed = buf[3];
                if (pressed == 1)
                    model->pressed[model->lookup[row * model->cols + col]]= true;
                else if (pressed == 0)
                    model->pressed[model->lookup[row * model->cols + col]]= false;
            } else if (buf[0] == ID_CUSTOM_LAYER_EVENT)
            {
                shared->active_layer = buf[1];

            }
        }
    }
}
