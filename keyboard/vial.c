#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
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
    S32 res; 
    U8 attempt;
    bool is_valid;
    bool all_zero;

    VIA_settle_delay(2000);

    for (attempt = 0; attempt < 5; attempt++)
    {
        VIA_flush_response(device);
        res = hid_write(device, req, RAW_HID_PACKET_SIZE + 1);

        if (res != VIA_PACKET_SIZE)
            continue;

        VIA_settle_delay(2000);

        memset(resp, 0, VIA_PACKET_SIZE);
        res = hid_read_timeout(device, resp, VIA_PACKET_SIZE, VIA_READ_TIMEOUT);
        if (res <= 0)
            continue;

        all_zero = true;
        for (int i = 0; i < res; i++)
        {
            if (resp[i] != 0)
            {
                all_zero = false;
                break;
            }
        }

        if (all_zero)
            continue;

        is_valid = true;
        break;
    }

    if (!is_valid)
        return false;

    return true;
}

bool VIA_confirm_protocol_version(KBS_model *model)
{
    U8 req[VIA_PACKET_SIZE] = {0};
    U8 resp[VIA_PACKET_SIZE] = {0};

    req[1] = VIA_GET_PROTOCOL_VERSION;


    if(!VIA_send_and_recieve(model->device, req, resp))
        return false;

    return true;
}

bool VIA_get_layers_count(KBS_model *model)
{
    U8 req[VIA_PACKET_SIZE] = {0};
    U8 resp[VIA_PACKET_SIZE] = {0};

    req[1] = VIA_GET_LAYER_COUNT;


    if(!VIA_send_and_recieve(model->device, req, resp))
        return false;

    model->layers_count = resp[1];

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

bool VIAL_get_def_size(KBS_model *model, U32 *size)
{
    U8 req[RAW_HID_PACKET_SIZE + 1] = {0};
    U8 resp[RAW_HID_PACKET_SIZE + 1] = {0};

    req[1] = VIAL_ESCAPE_BYTE;
    req[2] = VIAL_GET_KEYBOARD_DEFINITION_SIZE;

    if(!VIA_send_and_recieve(model->device, req, resp))
        return false;

    if (resp[0] == 0)
        return false;

    memcpy(size, resp, 4);

    return true;
}

bool VIAL_get_def(KBS_model *model, U8 *def_compressed, U32 def_size)
{
    U8 req[RAW_HID_PACKET_SIZE + 1] = {0};
    U8 resp[RAW_HID_PACKET_SIZE + 1] = {0};
    S32 res;
    U32 total_res = 0;
    U32 remaining = def_size;
    U32 block_count = 0;
    U32 copy_len = 0;
    U8 attempt;
    bool is_valid;
    bool all_zero;

    req[1] = VIAL_ESCAPE_BYTE;
    req[2] = VIAL_GET_KEYBOARD_DEFINITION;

    while (total_res < def_size)
    {
        req[3] = (block_count >>  0) & 0xFF;
        req[4] = (block_count >>  8) & 0xFF;
        req[5] = (block_count >> 16) & 0xFF;
        req[6] = (block_count >> 24) & 0xFF;

        is_valid = false;

        for (attempt = 0; attempt < 5; attempt++)
        {
            VIA_flush_response(model->device);
            
            res = hid_write(model->device, req, VIA_PACKET_SIZE);
            if (res != VIA_PACKET_SIZE)
                continue;

            VIA_settle_delay(2000);

            memset(resp, 0, VIA_PACKET_SIZE);
            res = hid_read_timeout(model->device, resp, VIA_PACKET_SIZE, VIA_READ_TIMEOUT);
            if (res <= 0)
                continue;

            all_zero = true;
            for (int i = 0; i < res; i++)
            {
                if (resp[i] != 0)
                {
                    all_zero = false;
                    break;
                }
            }

            if (all_zero)
                continue;

            is_valid = true;
            break;
        }

        if (!is_valid)
        {
            return false;
        }

        block_count++;
        copy_len = (U32)res < remaining ? (U32)res : remaining;
        memcpy(def_compressed + total_res, resp, copy_len);
        total_res += copy_len;
        remaining -= copy_len;
    }
    return true;
}

bool VIAL_get_keymap(KBS_model *model, U8 *keymap_buf, U32 keymap_size)
{
    U8 req[RAW_HID_PACKET_SIZE + 1] = {0};
    U8 resp[RAW_HID_PACKET_SIZE + 1] = {0};
    S32 res;
    U32 offset = 0;
    U16 resp_offset;
    U8 attempt;
    U8 chunk_size;

    bool is_valid;
    bool all_zero;

    req[1] = VIA_GET_KEYMAP_BUFFER;

    while (offset < keymap_size)
    {
        chunk_size = (keymap_size - offset) < MAX_CHUNK_SIZE 
            ? (U8)(keymap_size - offset) 
            : MAX_CHUNK_SIZE;

        req[2] = (offset >>  8) & 0xFF;
        req[3] = offset & 0xFF;
        req[4] = chunk_size;

        is_valid = false;

        for (attempt = 0; attempt < 5; attempt++)
        {
            VIA_flush_response(model->device);
            res = hid_write(model->device, req, RAW_HID_PACKET_SIZE + 1);

            if (res != VIA_PACKET_SIZE)
                continue;

            VIA_settle_delay(2000);

            memset(resp, 0, VIA_PACKET_SIZE);
            res = hid_read_timeout(model->device, resp, VIA_PACKET_SIZE, VIA_READ_TIMEOUT);
            if (res <= 0)
                continue;

            resp_offset = (resp[1] << 8) | resp[2];
            if (resp[0] != VIA_GET_KEYMAP_BUFFER || resp_offset != offset)
            {
                continue;
            }

            all_zero = true;
            for (int i = 0; i < res; i++)
            {
                if (resp[i] != 0)
                {
                    all_zero = false;
                    break;
                }
            }

            if (all_zero)
                continue;

            is_valid = true;
            break;
        }

        if (!is_valid)
            return false;

        memcpy(keymap_buf + offset, resp + 4, chunk_size);
        offset += chunk_size;
    }
    return true;
}

