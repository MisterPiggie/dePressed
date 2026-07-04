#include "keyboard/vial.h"
#include "keyboard/hid.h"
#include "keyboard/init.h"
#include "keyboard/decode.h"

bool KBS_connect_keyboard(App *app)
{
    KBS_model *model = &app->keyboards[app->active_model_idx];
    U32 def_size;
    U32 json_size;
    U8 *decompressed_json;

    if (!HID_open_device(model))
    {
        printf("failed open\n");
        return false;
    }
    if (!VIA_confirm_protocol_version(model))
    {
        printf("failed protocol\n");
        return false;
    }
    if (!VIA_get_layers_count(model))
    {
        printf("failed layers count\n");
        return false;
    }
    if (!VIAL_enabled(model))
    {
        printf("failed enabled\n");
        return false;
    }

    model->firmware = VIAL;

    if (!VIAL_get_def_size(model, &def_size))
    {
        printf("failed get size\n");
        return false;
    }

    U8 def_compressed[def_size];

    if (!VIAL_get_def(model, def_compressed, def_size))
    {
        printf("failed def\n");
        return false;
    }

    printf("Def size: %u\n", def_size);
    for (int i = 0; i < def_size; i++)
    {
        printf("%02x", def_compressed[i]);
    }

    decompressed_json = XZ_decode(def_compressed, def_size, &json_size);
    printf("JSON size: %u\n", json_size);
    for (int i = 0; i < json_size; i++)
    {
        printf("%c", decompressed_json[i]);
    }

    if (!decompressed_json) 
        return false;

    cJSON *json = JSON_parse_buffer(decompressed_json, json_size);
    if (!json)
    {
        printf("JSON\n");
        return false;
    }

    cJSON *matrix = cJSON_GetObjectItemCaseSensitive(json, "matrix");
    if(!cJSON_IsObject(matrix))
    {
        printf("matrix\n");
        return false;
    }
    
    cJSON *cols = cJSON_GetObjectItemCaseSensitive(matrix, "cols");
    if (!cJSON_IsNumber(cols))
    {
        printf("cols\n");
        return false;
    }
    
    cJSON *rows = cJSON_GetObjectItemCaseSensitive(matrix, "rows");
    if (!cJSON_IsNumber(rows))
    {
        printf("rows\n");
        return false;
    }
    model->rows = rows->valueint;

    model->cols = cols->valueint;

    cJSON *json_keymap = cJSON_GetObjectItemCaseSensitive(json, "keymap");
    if (!cJSON_IsArray(json_keymap))
    {
        printf("json keymap\n");
        return false;
    }

    U32 keymap_size = model->cols * model->rows * model->layers_count * 2;
    U8 keymap_buf[keymap_size];

    if(!VIAL_get_keymap(model, keymap_buf, keymap_size))
        return false;

    return true;
}
