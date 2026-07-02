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


    printf("Protocol version:\n");
    VIA_get_protocol_version(&app.keyboards[app.active_model_idx]);
    printf("Vial check:\n");
    VIAL_enabled(&app.keyboards[app.active_model_idx]);
    printf("Def size:\n");
    VIAL_get_def_size(&app.keyboards[app.active_model_idx]);

}

