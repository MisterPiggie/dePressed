#include "core/types.h"
#include "deps/cJSON.h"

U8 *XZ_decode(const U8 *input, U32 input_len, U32 *out_len);
cJSON *JSON_parse_buffer(const U8 *buffer, U32 buffer_len);

// const char *keycodes[256] = 
// {
//     "", "", "", "", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K",
//     "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
//     "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
//     "Enter", "Esc", "Bksp", "Tab", "Space", "-", "=", "[", "]", "\\", ";", "'",
//     "`", ",", ".", "/", "Caps", 
//     "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
//     "Ins", "Home", "PgUp", "Del", "End", "PgDn", "Right", "Left", "Down", "Up",
//     "LCtl", "LSft", "LAlt", "LGui", "RCtl", "RSft", "RAlt", "RGui",
// };
