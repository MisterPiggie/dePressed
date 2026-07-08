#include <hidapi/hidapi.h>
#include <unistd.h>
#include "core/types.h"
#include "keyboard/hid.h"
#include "keyboard/vial.h"
#include "keyboard/init.h"

int main(void)
{
    App app = {0};
    app.arena = arena_create(MB(100), KB(32));
    app.keyboards_count = 0;

    HID_get_suitable_keyboards(&app);
    if (app.keyboards_count == 0)
        //generate no keyboard found
        return 0;
    else if (app.keyboards_count == 1)
        app.active_model_idx = 0;
    else 
        //generate choose keyboard
        return 0;

    KBS_connect_keyboard(&app);

    listen_for_keypresses(&app.keyboards[app.active_model_idx]);

}

