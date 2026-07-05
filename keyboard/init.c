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

    decompressed_json = XZ_decode(def_compressed, def_size, &json_size);
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
    
    cJSON *rows = cJSON_GetObjectItemCaseSensitive(matrix, "rows");
    if (!cJSON_IsNumber(rows))
    {
        printf("rows\n");
        return false;
    }

    cJSON *cols = cJSON_GetObjectItemCaseSensitive(matrix, "cols");
    if (!cJSON_IsNumber(cols))
    {
        printf("cols\n");
        return false;
    }

    model->rows = rows->valueint;
    model->cols = cols->valueint;
    model->layout.key_count = model->rows * model->cols;

    cJSON *json_layouts = cJSON_GetObjectItemCaseSensitive(json, "layouts");
    if (!cJSON_IsObject(json_layouts))
    {
        printf("json layouts\n");
        return false;
    }

    cJSON *json_keymap = cJSON_GetObjectItemCaseSensitive(json_layouts, "keymap");
    if (!cJSON_IsArray(json_keymap))
    {
        printf("json keymap\n");
        return false;
    }

    model->layout.keys = arena_push_array(&app->arena, KBS_key, model->layout.key_count);

    KBS_cursor cursor = {0};
    cursor.y = -1;
    cursor.width = 1;
    cursor.height = 1;

    cJSON *row;
    U8 key_idx = 0;
    printf("Parse started\n");

    cJSON_ArrayForEach(row, json_keymap)
    {
        if (!cJSON_IsArray(row))
            continue;

        printf("Row selected\n");

        cursor.y += 1;
        cursor.x = cursor.rotation_x;

        cJSON *item;
        cJSON_ArrayForEach(item, row)
        {
            if (cJSON_IsObject(item))
                apply_offset(&cursor, item);
            else if (cJSON_IsString(item))
            {
                printf("Key added\n");
                KBS_key *key = &model->layout.keys[key_idx];
                key->x = cursor.x;
                key->y = cursor.y;
                key->width = cursor.width;
                key->height = cursor.height;
                key->angle = cursor.rotation_angle;
                key->rx = cursor.rotation_x;
                key->ry = cursor.rotation_y;

                parse_row_col(item->valuestring, &key->row, &key->col);

                cursor.x += cursor.width;
                cursor.width = 1;
                cursor.height = 1;
                key_idx++;
            }
        }
    }

    for (int i = 0; i < model->layout.key_count; i++)
    {
        KBS_key key = model->layout.keys[i];
        printf("Key idx: %d; X = %f; Y = %f\n", i, key.x, key.y);
        printf("Row = %c; Col = %c\n", key.row, key.col);
        printf("Angle = %f; RX = %f; RY = %f\n\n", key.angle, key.rx, key.ry);
    }

    U32 keymap_size = model->cols * model->rows * model->layers_count * 2;
    U8 keymap_buf[keymap_size];

    if(!VIAL_get_keymap(model, keymap_buf, keymap_size))
        return false;

    return true;
}

static void apply_offset(KBS_cursor *cursor, cJSON *item)
{
    cJSON *v;
    if ((v = cJSON_GetObjectItem(item, "x")))  
        cursor->x += v->valuedouble;
    if ((v = cJSON_GetObjectItem(item, "y")))  
        cursor->y += v->valuedouble;

    if ((v = cJSON_GetObjectItem(item, "w")))  
        cursor->width  = v->valuedouble;
    if ((v = cJSON_GetObjectItem(item, "h")))  
        cursor->height = v->valuedouble;

    if ((v = cJSON_GetObjectItem(item, "r")))  
        cursor->rotation_angle  = v->valuedouble;

    if ((v = cJSON_GetObjectItem(item, "rx"))) 
    { 
        cursor->rotation_x = v->valuedouble; 
        cursor->x = cursor->rotation_x; 
    }
    if ((v = cJSON_GetObjectItem(item, "ry"))) 
    {
        cursor->rotation_y = v->valuedouble;
        cursor->y = cursor->rotation_y; 
    }
}

static void parse_row_col(const char *label, U8 *row, U8 *col)
{
    char buf[32];
    const char *nl = strchr(label, '\n');
    size_t len = nl ? (size_t)(nl - label) : strlen(label);
    if (len >= sizeof(buf)) 
        len = sizeof(buf) - 1;
    memcpy(buf, label, len);

    buf[len] = '\0';
    sscanf(buf, "%c,%c", row, col);
}

