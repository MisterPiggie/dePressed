#include "core/types.h"
#include <stdbool.h>

bool HID_get_suitable_keyboards(App *app);
void HID_device_info_to_KBS_model(Arena *arena, struct hid_device_info *info, KBS_model *model);
