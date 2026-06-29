#include "core/types.h"
#include <stdbool.h>

#define RAW_HID_PACKET_SIZE 64
#define HID_USAGE_PAGE 0xFF60

bool HID_get_suitable_keyboards(App *app);
void HID_device_info_to_KBS_model(Arena *arena, struct hid_device_info *info, KBS_model *model);
bool HID_open_device(KBS_model *model);
