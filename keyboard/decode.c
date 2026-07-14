#include <lzma.h>
#include <stdlib.h>
#include "keyboard/decode.h"

U8 *XZ_decode(const U8 *input, U32 input_len, U32 *out_len)
{
    U32 out_cap = input_len * 20; 
    U64 memlimit = UINT64_MAX;
    U64 in_pos = 0, out_pos = 0;
    U8 *out_buf = malloc(out_cap);
    if (!out_buf)
    {
        printf("malloc\n");
        return NULL;
    }

    lzma_ret ret = lzma_stream_buffer_decode(
            &memlimit, 0, NULL,
            input, &in_pos, input_len,
            out_buf, &out_pos, out_cap
            );

    if (ret != LZMA_OK) 
    {
        free(out_buf);
        printf("LZMA_ERROR code %d\n", ret);
        return NULL;
    }

    *out_len = out_pos;
    return out_buf;
}

cJSON *JSON_parse_buffer(const U8 *buffer, U32 buffer_len)
{
    cJSON *root = cJSON_ParseWithLength((char *)buffer, buffer_len);

    if (root == NULL)
        return NULL;

    return root;
}

