#include "keyboard/vial.h"
#include "keyboard/hid.h"
#include "keyboard/init.h"
#include "keyboard/decode.h"

bool KBS_connect_keyboard(App *app)
{
    KBS_model *model = &app->keyboards[app->active_model_idx];
    U32 def_size;
    U32 json_size;
    U8 *json;

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
    {
        printf("failed def\n");
        return false;
    }

    printf("Def size: %u\n", def_size);
    for (int i = 0; i < def_size; i++)
    {
        printf("%02x", def_compressed[i]);
    }

    json = XZ_decode(def_compressed, def_size, &json_size);

    if (!json) 
        return false;

    return true;
}
