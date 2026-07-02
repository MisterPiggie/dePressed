#include <hidapi/hidapi.h>
#include <stdio.h>
#include <unistd.h>
#include "core/types.h"
#include "keyboard/hid.h"
#include "keyboard/vial.h"

int main(void)
{
    App app = {0};
    app.arena = arena_create(MB(100), KB(32));
    app.keyboards_count = 0;

    if (!HID_get_suitable_keyboards(&app))
    {
        printf("no keybords were found\n");
        return -1;
    }

    for (int i = 0; i < app.keyboards_count; i++)
    {
        printf("Name: %ls\n", app.keyboards[i].product_name);
        printf("Path: %s\n", app.keyboards[i].path);
    }

    for (int i = 0; i < app.keyboards_count; i++)
    {
        app.active_model_idx = i;
        if(HID_open_device(&app.keyboards[i]))
        {
            printf("Connected\n");
            break;
        }
    }


    printf("Protocol version:\n");
    VIA_get_protocol_version(&app.keyboards[app.active_model_idx]);
    printf("Vial check:\n");
    VIAL_enabled(&app.keyboards[app.active_model_idx]);
    printf("Def size:\n");
    VIAL_get_def_size(&app.keyboards[app.active_model_idx]);

}

