#include "core/types.h"
#include "deps/cJSON.h"

#define BEETWEEN_KEY_PADDING 0.1f
#define MARGIN 0.5f

bool KBS_connect_keyboard(App *app);
static void apply_offset(KBS_cursor *cursor, cJSON *item);
static void parse_row_col(const char *label, U8 *row, U8 *col);
static void KBS_get_bounds(App *app);
static bool is_multiline_label(const char *label);
void *KBS_key_listener_thread(void *arg);
bool KBS_start_key_listener(App *app);
void KBS_disconnect_keyboard(App *app);
S8 KBS_resolve_layer(KBS_key *key, U32 layer_state);
