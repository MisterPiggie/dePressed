#include "core/types.h"
#include "deps/cJSON.h"

U8 *XZ_decode(const U8 *input, U32 input_len, U32 *out_len);
cJSON *JSON_parse_buffer(const U8 *buffer, U32 buffer_len);
