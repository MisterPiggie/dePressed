#include "core/types.h"
#include <stdbool.h>

#define RAW_HID_PACKET_SIZE 64
#define VIA_PACKET_SIZE (RAW_HID_PACKET_SIZE + 1)
#define VIA_READ_TIMEOUT 100
#define HID_USAGE_PAGE 0xFF60
#define ID_CUSTOM_KEY_EVENT 0x77
#define ID_CUSTOM_LAYER_EVENT 0x20

bool HID_get_suitable_keyboards(App *app);
void HID_device_info_to_KBS_model(Arena *arena, struct hid_device_info *info, KBS_model *model);
bool HID_open_device(KBS_model *model);
void listen_for_keypresses(KBS_model *model, App_shared *shared);
