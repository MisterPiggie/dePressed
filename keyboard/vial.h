#include <stdbool.h>
#include <hidapi/hidapi.h>
#include "core/num_types.h"
#include "core/types.h"


#define VIA_GET_PROTOCOL_VERSION            0x01
#define VIA_GET_KEYCODE                     0x02
#define VIA_GET_KEYMAP_BUFFER               0x12


#define VIAL_ESCAPE_BYTE                    0xFE

#define VIAL_GET_KEYBOARD_ID                0x00
#define VIAL_GET_KEYBOARD_DEFINITION_SIZE   0x01
#define VIAL_GET_KEYBOARD_DEFINITION        0x02


static void VIA_flush_response(hid_device *device);
static bool VIA_send_and_recieve(hid_device *device, const U8 *req, U8 *resp);
static void VIA_settle_delay(U32 microseconds);


bool VIA_get_protocol_version(KBS_model *model);
bool VIAL_get_def_size(KBS_model *model);
bool VIAL_enabled(KBS_model *model);
