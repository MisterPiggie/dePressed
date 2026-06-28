#include <hidapi/hidapi.h>
#include <stdio.h>
#include "key_capture/find_keyboard.h"
#include "core/types.h"
#include "core/constants.h"

int main(void)
{
    app app;
    app.arena = arena_create(MB(100), KB(32));
    app.kbs = arena_push_array(&app.arena, via_keyboard, MAX_KBS);
    app.kbs_count = 0;
    
    if (!scan_for_keyboards(&app))
    {
        printf("ERROR NO KBS FOUND\n");
    }

    printf("Path: %s\n", app.kbs->path);
    printf("Product: %ls\n", app.kbs->product);

    get_layer_state(&app);
}

