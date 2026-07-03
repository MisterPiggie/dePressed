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
        return false;
    if (!VIA_confirm_protocol_version(model))
        return false;
    if (!VIAL_enabled(model))
        return false;

    model->firmware = VIAL;

    if (!VIAL_get_def_size(model, &def_size))
        return false;

    U8 def_compressed[def_size];

    if (!VIAL_get_def(model, def_compressed, def_size))
        return false;

    json = XZ_decode(def_compressed, def_size, &json_size);

    if (!json) 
        return false;

    return true;
}
