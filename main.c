#include <hidapi/hidapi.h>
#include <stdio.h>
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
    KBS_connect_keyboard(&app);

}

