#include <stdio.h>
#include "vial.h"
#include "hid.h"


bool VIA_send_and_recieve(hid_device *device, const U8 *req, U8 *resp)
{
    int res = hid_write(device, req, RAW_HID_PACKET_SIZE + 1);
    if (res != RAW_HID_PACKET_SIZE + 1)
        return false;

    printf("Request: ");
    for (int i = 0; i < 65; i++)
    {
        printf("%.02x", req[i]);
    }
    printf("\n");

    memset(resp, 0, RAW_HID_PACKET_SIZE + 1);
    res = hid_read(device, resp, RAW_HID_PACKET_SIZE + 1);

    if (res <= 0)
        return false;

    printf("Response: ");
    for (int i = 0; i < res; i++)
    {
        printf("%.02x", resp[i]);
    }
    printf("\n");

    return true;
}

int VIA_get_layer_count(hid_device *device)
{
    U8 req[RAW_HID_PACKET_SIZE + 1] = {0};
    U8 resp[RAW_HID_PACKET_SIZE + 1] = {0};

    req[1] = VIA_GET_PROTOCOL_VERSION;

    if(!VIA_send_and_recieve(device, req, resp))
        return -1;
    

    return 0;
}


bool VIAL_enabled(hid_device *device)
{
    U8 req[RAW_HID_PACKET_SIZE + 1] = {0};
    U8 resp[RAW_HID_PACKET_SIZE + 1] = {0};

    req[1] = VIAL_ESCAPE_BYTE;

    if(!VIA_send_and_recieve(device, req, resp))
        return false;
    
    if (resp[1] != VIAL_ESCAPE_BYTE || resp[2] != 0x00)
        return false;

    return true;
}

int VIAL_get_def_size(hid_device *device)
{
    U8 req[RAW_HID_PACKET_SIZE + 1] = {0};
    U8 resp[RAW_HID_PACKET_SIZE + 1] = {0};

    req[1] = VIAL_ESCAPE_BYTE;
    req[2] = VIAL_GET_KEYBOARD_DEFINITION_SIZE;

    if(!VIA_send_and_recieve(device, req, resp))
        return -1;
    

    return 0;
}
