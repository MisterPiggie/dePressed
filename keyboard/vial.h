#include <stdbool.h>
#include <hidapi/hidapi.h>
#include "core/num_types.h"


#define VIA_GET_PROTOCOL_VERSION            0x01
#define VIA_GET_KEYCODE                     0x02
#define VIA_GET_LAYER_COUNT                 0x11
#define VIA_GET_KEYMAP_BUFFER               0x12


#define VIAL_ESCAPE_BYTE                    0xFE

#define VIAL_GET_KEYBOARD_ID                0x00
#define VIAL_GET_KEYBOARD_DEFINITION_SIZE   0x01
#define VIAL_GET_KEYBOARD_DEFINITION        0x02


void VIA_flush_response(hid_device *device);
bool VIA_send_and_recieve(hid_device *device, const U8 *req, U8 *resp);
int VIA_get_layer_count(hid_device *device);
int VIAL_get_def_size(hid_device *device);
bool VIAL_enabled(hid_device *device);

static void VIA_settle_delay(U32 microseconds);
