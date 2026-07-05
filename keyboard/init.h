#include "core/types.h"
#include "deps/cJSON.h"

bool KBS_connect_keyboard(App *app);
static void apply_offset(KBS_cursor *cursor, cJSON *item);
static void parse_row_col(const char *label, U8 *row, U8 *col);
