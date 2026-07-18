#include <lzma.h>
#include <stdlib.h>
#include "keyboard/decode.h"
#include "GUI/error.h"

U8 *XZ_decode(App *app, const U8 *input, U32 input_len, U32 *out_len)
{
    U32 out_cap = input_len * 20; 
    U64 memlimit = UINT64_MAX;
    U64 in_pos = 0, out_pos = 0;
    U8 *out_buf = malloc(out_cap);
    if (!out_buf)
    {
        GUI_create_an_error(app, ": not enough memory for decompression");
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

        char lzma_err[50];        
        sprintf(lzma_err, ": LZMA_ERROR code: %d", ret);

        GUI_create_an_error(app, lzma_err);
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

const char *decode_keycode(U16 kc, char *buf, U32 buf_size)
{
    if (kc == 0x0000) 
        return "";        
    if (kc == 0x0001) 
        return "";      

    static const char *basic[256] = {0};
    if (basic[0x04] == NULL)  
    {
        basic[0x04]="A",basic[0x05]="B",basic[0x06]="C",basic[0x07]="D",basic[0x08]="E",basic[0x09]="F",
        basic[0x0A]="G",basic[0x0B]="H",basic[0x0C]="I",basic[0x0D]="J",basic[0x0E]="K",basic[0x0F]="L",
        basic[0x10]="M",basic[0x11]="N",basic[0x12]="O",basic[0x13]="P",basic[0x14]="Q",basic[0x15]="R",
        basic[0x16]="S",basic[0x17]="T",basic[0x18]="U",basic[0x19]="V",basic[0x1A]="W",basic[0x1B]="X",
        basic[0x1C]="Y",basic[0x1D]="Z",

        basic[0x1E]="1",basic[0x1F]="2",basic[0x20]="3",basic[0x21]="4",basic[0x22]="5",
        basic[0x23]="6",basic[0x24]="7",basic[0x25]="8",basic[0x26]="9",basic[0x27]="0",

        basic[0x28]="Enter", basic[0x29]="Esc", basic[0x2A]="Bksp", basic[0x2B]="Tab", basic[0x2C]="Space", basic[0x2D]="-",
        basic[0x2E]="=", basic[0x2F]="[", basic[0x30]="]", basic[0x31]="\\", basic[0x32]="#", basic[0x33]=";", basic[0x34]="'",
        basic[0x35]="`", basic[0x36]=",", basic[0x37]=".", basic[0x38]="/", basic[0x39]="Caps",

        basic[0x3A]="F1",basic[0x3B]="F2",basic[0x3C]="F3",basic[0x3D]="F4", basic[0x3E]="F5",basic[0x3F]="F6",
        basic[0x40]="F7",basic[0x41]="F8",basic[0x42]="F9",basic[0x43]="F10",basic[0x44]="F11",basic[0x45]="F12",

        basic[0x46]="PrtSc",  basic[0x47]="Scroll", basic[0x48]="Pause",
        basic[0x49]="Ins",    basic[0x4A]="Home",   basic[0x4B]="PgUp",
        basic[0x4C]="Del",    basic[0x4D]="End",    basic[0x4E]="PgDn",
        basic[0x4F]="Right",  basic[0x50]="Left",   basic[0x51]="Down",  basic[0x52]="Up",

        basic[0x53]="NumLk",  basic[0x54]="P/",   basic[0x55]="P*",  basic[0x56]="P-",
        basic[0x57]="P+",     basic[0x58]="PEnt",
        basic[0x59]="P1",     basic[0x5A]="P2",   basic[0x5B]="P3",basic[0x5C]="P4",basic[0x5D]="P5",
        basic[0x5E]="P6",     basic[0x5F]="P7",   basic[0x60]="P8",basic[0x61]="P9",basic[0x62]="P0",
        basic[0x63]="P.",     basic[0x64]="NUBS", 
        basic[0x65]="Menu",   basic[0x66]="Power",
        basic[0x67]="P=",

        basic[0x68]="F13",basic[0x69]="F14",basic[0x6A]="F15",basic[0x6B]="F16",
        basic[0x6C]="F17",basic[0x6D]="F18",basic[0x6E]="F19",basic[0x6F]="F20",
        basic[0x70]="F21",basic[0x71]="F22",basic[0x72]="F23",basic[0x73]="F24",

        basic[0x74]="Exec",  basic[0x75]="Help",  basic[0x76]="Menu",  basic[0x77]="Select",
        basic[0x78]="Stop",  basic[0x79]="Again", basic[0x7A]="Undo",  basic[0x7B]="Cut",
        basic[0x7C]="Copy",  basic[0x7D]="Paste", basic[0x7E]="Find",  basic[0x7F]="Mute",
        basic[0x80]="VolUp", basic[0x81]="VolDn",

        basic[0x82]="LockCL", basic[0x83]="LockNL", basic[0x84]="LockSL",

        basic[0x85]="P,",    
        basic[0x87]="INT1",  basic[0x88]="INT2",  basic[0x89]="INT3",  basic[0x8A]="INT4",
        basic[0x8B]="INT5",  basic[0x8C]="INT6",  basic[0x8D]="INT7",  basic[0x8E]="INT8",
        basic[0x8F]="INT9",  basic[0x90]="LNG1",  basic[0x91]="LNG2",  basic[0x92]="LNG3",
        basic[0x93]="LNG4",  basic[0x94]="LNG5",  basic[0x95]="LNG6",  basic[0x96]="LNG7",
        basic[0x97]="LNG8",  basic[0x98]="LNG9",

        basic[0x99]="Eras", basic[0x9A]="Req", basic[0x9B]="Cancel", basic[0x9C]="Clear", basic[0x9D]="Prior", 
        basic[0x9E]="Return", basic[0x9F]="Separ", basic[0xA0]="Out", basic[0xA1]="Oper", basic[0xA2]="ClearA", 
        basic[0xA3]="CrSel", basic[0xA4]="ExSel", basic[0xA5]="Power", basic[0xA6]="Sleep", basic[0xA7]="Wake", 
        basic[0xA8]="Mute", basic[0xA9]="VolUp", basic[0xAA]="VolDn", basic[0xAB]="NextTr", basic[0xAC]="PrevTr", 
        basic[0xAD]="Stop", basic[0xAE]="Pause", basic[0xAF]="Select", basic[0xB0]="Eject", basic[0xB1]="Mail", 
        basic[0xB2]="Calc", basic[0xB3]="MyPC", 


        basic[0xB4]="WSearch", basic[0xB5]="WHome", basic[0xB6]="WBack", basic[0xB7]="WFor", basic[0xB8]="WStop", 
        basic[0xB9]="WRefr", basic[0xBA]="WFav", basic[0xBB]="FFov", basic[0xBC]="Rewind", basic[0xBD]="BrghUp", 
        basic[0xBE]="BrghDn", basic[0xBF]="CtrlPn", basic[0xC0]="Assist", basic[0xC1]="MsCtrl", basic[0xC2]="LnPad", 

        basic[0xCD]="MsUp", basic[0xCE]="MsDn", basic[0xCF]="MsLft", basic[0xD0]="MsRght", basic[0xD1]="MsBt1", 
        basic[0xD2]="MsBt2", basic[0xD3]="MsBt3", basic[0xD4]="MsBt4", basic[0xD5]="MsBt5", basic[0xD6]="MsBt6", 
        basic[0xD7]="MsBt7", basic[0xD8]="MsBt8", basic[0xD9]="MsWhUp", basic[0xDA]="MsWhDn", basic[0xDB]="MsWhLf", 
        basic[0xD7]="MsBt7", basic[0xD8]="MsBt8", basic[0xD9]="MsWhUp", basic[0xDA]="MsWhDn", basic[0xDB]="MsWhLf", 
        basic[0xDC]="MsWhRt", basic[0xDD]="MsAc0", basic[0xDE]="MsAc1", basic[0xDF]="MsAc2", 

        basic[0xE0]="LCtrl", basic[0xE1]="Shift", basic[0xE2]="LAlt", basic[0xE3]="LGui",
        basic[0xE4]="RCtrl", basic[0xE5]="Shift", basic[0xE6]="RAlt", basic[0xE7]="RGui";
    }

    if (kc < 256 && basic[kc])
        return basic[kc];

    if (kc >= 0x0100 && kc <= 0x1FFF)
    {
        U8 mods = (kc >> 8) & 0x1F;
        U8 base_kc = kc & 0xFF;

        if (mods == 0x02 || mods == 0x12)  
        {
            const char *symbol = shifted_symbol(base_kc);
            if (symbol) 
                return symbol;   
        }

        if (!basic[base_kc])
        {
            snprintf(buf, buf_size, "OSL(%d)", kc - 0x5280);
            return buf;
        }

        bool is_right = (mods & 0x10) != 0;
        char mod_str[16] = "";

        if (mods & 0x01) 
            strcat(mod_str, is_right ? "RC-" : "LC-");
        if (mods & 0x02) 
            strcat(mod_str, is_right ? "RS-" : "LS-");
        if (mods & 0x04) 
            strcat(mod_str, is_right ? "RA-" : "LA-");
        if (mods & 0x08) 
            strcat(mod_str, is_right ? "RG-" : "LG-");

        snprintf(buf, buf_size, "%s%s", mod_str, basic[base_kc]);
        return buf;
    }

    if (kc >= 0x5220 && kc <= 0x523F) 
    { 
        snprintf(buf, buf_size, "MO(%d)", kc - 0x5220);
        return buf; 
    }

    if (kc >= 0x5200 && kc <= 0x521F) 
    {
        snprintf(buf, buf_size, "TO(%d)", kc - 0x5200);
        return buf;
    }

    if (kc >= 0x5240 && kc <= 0x525F) 
    {
        snprintf(buf, buf_size, "DF(%d)", kc - 0x5240);
        return buf;
    }

    if (kc >= 0x5260 && kc <= 0x527F) 
    {
        snprintf(buf, buf_size, "TG(%d)", kc - 0x5260);
        return buf;
    }

    if (kc >= 0x5280 && kc <= 0x529F) 
    {
        snprintf(buf, buf_size, "OSL(%d)", kc - 0x5280);
        return buf;
    }

    snprintf(buf, buf_size, "0x%04X", kc);
    return buf;
}

static const char *shifted_symbol(U8 base_kc)
{
    switch (base_kc)
    {
        case 0x1E: 
            return "!";  
        case 0x1F: 
            return "@";  
        case 0x20: 
            return "#";  
        case 0x21: 
            return "$";  
        case 0x22: 
            return "%";  
        case 0x23: 
            return "^";  
        case 0x24: 
            return "&"; 
        case 0x25: 
            return "*";
        case 0x26: 
            return "(";  
        case 0x27: 
            return ")"; 
        case 0x2D: 
            return "_";
        case 0x2E: 
            return "+"; 
        case 0x2F: 
            return "{";
        case 0x30: 
            return "}";  
        case 0x31: 
            return "|";  
        case 0x33: 
            return ":";  
        case 0x34: 
            return "\""; 
        case 0x35: 
            return "~";  
        case 0x36: 
            return "<";  
        case 0x37: 
            return ">";  
        case 0x38: 
            return "?";  
        default: 
            return NULL;   
    }
}
