#include <hidapi/hidapi.h>
#include "keyboard/hid.h"
#include "core/constants.h"

bool HID_get_suitable_keyboards(App *app)
{
    struct hid_device_info *device_linked_list = hid_enumerate(0, 0);
    if (device_linked_list == NULL)
        return false;
    struct hid_device_info *node = device_linked_list;

    while (node != NULL)
    {
        if (node->usage_page == VIAL_USAGE_PAGE)
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
        if (node->usage_page == VIAL_USAGE_PAGE)
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

    model->firmware = QMK;
}
