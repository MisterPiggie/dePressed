#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include "vial.h"
#include "hid.h"

void VIA_flush_response(hid_device *device)
{
    U8 scratch[VIA_PACKET_SIZE];
    while (hid_read_timeout(device, scratch, sizeof(scratch), 0) > 0)
    {
    }
}

static void VIA_settle_delay(U32 microseconds)
{
    struct timespec ts;
    ts.tv_sec = microseconds / 1000000L;
    ts.tv_nsec = (microseconds % 1000000L) * 1000L;

    while (nanosleep(&ts, &ts) == -1)
    {
        if (errno != EINTR)
            break;
    }
}

bool VIA_send_and_recieve(hid_device *device, const U8 *req, U8 *resp)
{
    VIA_flush_response(device);
    int res = hid_write(device, req, RAW_HID_PACKET_SIZE + 1);

    if (res != VIA_PACKET_SIZE)
        return false;

    VIA_settle_delay(1000);

    printf("Request: ");
    for (int i = 0; i < 33; i++)
        printf("%.02x", req[i]);
    printf("\n");

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

bool VIA_get_protocol_version(KBS_model *model)
{
    U8 req[RAW_HID_PACKET_SIZE + 1] = {0};
    U8 resp[RAW_HID_PACKET_SIZE + 1] = {0};

    req[1] = VIA_GET_PROTOCOL_VERSION;

    if(!VIA_send_and_recieve(model->device, req, resp))
        return false;

    if (resp[0] != req[1])
        return false;

    return true;
}


bool VIAL_enabled(KBS_model *model)
{
    U8 req[RAW_HID_PACKET_SIZE + 1] = {0};
    U8 resp[RAW_HID_PACKET_SIZE + 1] = {0};

    req[1] = VIAL_ESCAPE_BYTE;

    if(!VIA_send_and_recieve(model->device, req, resp))
        return false;
    
    if (resp[0] == 0x00)
        return false;

    return true;
}

bool VIAL_get_def_size(KBS_model *model)
{
    U8 req[RAW_HID_PACKET_SIZE + 1] = {0};
    U8 resp[RAW_HID_PACKET_SIZE + 1] = {0};

    U32 def_size = 0;

    req[1] = VIAL_ESCAPE_BYTE;
    req[2] = VIAL_GET_KEYBOARD_DEFINITION_SIZE;

    if(!VIA_send_and_recieve(model->device, req, resp))
        return false;

    if (resp[0] == 0)
        return false;

    for (int i = 0; i < 4; i++)
    {
        def_size |= ((U32)resp[i]) << (i * 8);
    }

    printf("Def size: %u\n", def_size);

    return true;
}

bool VIAL_get_def(KBS_model *model, U8 *out)
{
    U8 req[RAW_HID_PACKET_SIZE + 1] = {0};
    U8 resp[RAW_HID_PACKET_SIZE + 1] = {0};

    U32 def_size = 0;

    req[1] = VIAL_ESCAPE_BYTE;
    req[2] = VIAL_GET_KEYBOARD_DEFINITION_SIZE;

    if(!VIA_send_and_recieve(model->device, req, resp))
        return false;

    if (resp[0] == 0)
        return false;

    for (int i = 0; i < 4; i++)
    {
        def_size |= ((U32)resp[i]) << (i * 8);
    }

    printf("Def size: %u\n", def_size);

    U8 def[def_size] = {0};

    return true;
}

