#include <hidapi/hidapi.h>
#include <stdbool.h>
#include "core/types.h"


bool scan_for_keyboards(app *app);
void get_layer_state(app *app);
bool set_kb(app *app, U8 pos);
void handle_incoming_report(U8 *buf, model_keyboard *model);
    
