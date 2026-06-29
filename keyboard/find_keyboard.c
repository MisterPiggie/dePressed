#include "find_keyboard.h"
#include <unistd.h>
#include "core/constants.h"

bool scan_for_keyboards(app *app)
{
    struct hid_device_info *list_of_kbs = hid_enumerate(0, 0); 
    struct hid_device_info *temp_kb = list_of_kbs;
    bool success = false;
    
    while(temp_kb != NULL)
    {
        if (temp_kb->usage_page == VIAL_USAGE_PAGE)
        {
            app->kbs[app->kbs_count].path = arena_push_str(&app->arena, temp_kb->path);
            app->kbs[app->kbs_count].product = arena_push_wstr(&app->arena, temp_kb->product_string);
            success = true;
        }
        temp_kb = temp_kb->next;
    }

    hid_free_enumeration(list_of_kbs);

    return success;
}

bool set_kb(app *app, U8 pos)
{

    if (pos >= MAX_KBS)
        return false;
    app->model->device = hid_open_path(app->kbs[pos].path);
    if (!app->model->device)
        return false;

    return true;
}

void get_layer_state(app *app)
{
    S32 res;
    U8  buf[64] = {0};
    U8  req[32] = {0};
    req[0] = 0xFE;
    req[1] = 0x00;

    while (1) 
    {
        res = hid_read(app->model->device, buf, sizeof(buf));

        if (res < 0)
        {
            printf("Ошибка при чтении из устройства.\n");
            break;
        }

        if (res > 0)
        {
            printf("%02x ", buf[1]);
            printf("\n");
        }
    }
    return;
}

void get_key_press(app *app)
{
    U8 buf[64];
    while (hid_read_timeout(app->model->device, buf, sizeof(buf), 0) > 0)
    {
        handle_incoming_report(buf, app->model);
    }
    return;
}

void handle_incoming_report(U8 *buf, model_keyboard *model)
{
    if (buf[0] == LAYER_EVENT)
    {
        model->current_layer = buf[1];
    }
    return;
}

