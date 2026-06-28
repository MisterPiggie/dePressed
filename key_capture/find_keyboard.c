#include "find_keyboard.h"
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

    return success;
}
