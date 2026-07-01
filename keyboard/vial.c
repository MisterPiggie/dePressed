#include <stdio.h>
#include <unistd.h>
#include "vial.h"
#include "hid.h"

void VIA_flush_response(hid_device *device)
{
    U8 scratch[VIA_PACKET_SIZE];
    while (hid_read_timeout(device, scratch, sizeof(scratch), 0 > 0))
    {
    }
}

bool VIA_send_and_recieve(hid_device *device, const U8 *req, U8 *resp)
{
    VIA_flush_response(device);
    int res = hid_write(device, req, RAW_HID_PACKET_SIZE + 1);

    if (res != VIA_PACKET_SIZE)
        return false;

    printf("Request: ");
    for (int i = 0; i < 65; i++)
        printf("%.02x", req[i]);
    printf("\n");

    sleep(1);

    memset(resp, 0, RAW_HID_PACKET_SIZE + 1);
    res = hid_read_timeout(device, resp, RAW_HID_PACKET_SIZE + 1, VIA_READ_TIMEOUT);

    if (res <= 0)
        return false;

    printf("Response: ");
    for (int i = 0; i < res; i++)
        printf("%.02x", resp[i]);
    printf("\n");

    return true;
}

int VIA_get_layer_count(hid_device *device)
{
    U8 req[RAW_HID_PACKET_SIZE + 1] = {0};
    U8 resp[RAW_HID_PACKET_SIZE + 1] = {0};

    req[1] = VIA_GET_LAYER_COUNT;

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
