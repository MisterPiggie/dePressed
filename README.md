# dePressed

A cross-platform overlay keyboard visualizer for VIAL QMK keyboards. It renders a live, on-screen overlay of your keyboard, helping you train touch-typing and easing the process of getting used to multi-layered keyboards.

# Usage example

![gif](https://github.com/MisterPiggie/dePressed/blob/main/depressed.gif)

## Setup

### 1. Find your keyboard's firmware

You'll need the QMK firmware source for your specific keyboard (from https://github.com/vial-kb/vial-qmk or your keyboard vendor's fork). Locate (or create) your keymap's `keymap.c` file, typically at:

```
vial-qmk/keyboards/<your_keyboard>/keymaps/<your_keymap>/keymap.c
```

### 2. Add the Raw HID hooks

Add the following code to your `keymap.c`. This adds two callback functions QMK will call automatically - one whenever the active layer changes, and one whenever any key is pressed or released 

```c
// Called automatically by QMK whenever the active layer changes.
layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t data[32] = {0};
    data[0] = 0x20;                  // packet ID: identifies this as a "layer state" packet
    data[1] = state & 0xFF;          // layer state, byte 0 (bits 0-7)
    data[2] = (state >> 8) & 0xFF;   // layer state, byte 1 (bits 8-15)
    data[3] = (state >> 16) & 0xFF;  // layer state, byte 2 (bits 16-23)
    data[4] = (state >> 24) & 0xFF;  // layer state, byte 3 (bits 24-31)
    raw_hid_send(data, sizeof(data)); // send the 32-byte HID report to the host app
    return state; // must return state unchanged so QMK actually applies the layer switch
}

// Called automatically by QMK on every key press/release event.
bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
    uint8_t report[32] = {0};
    report[0] = 0x77;                          // packet ID: identifies this as a "key event" packet
    report[1] = record->event.key.row;         // physical matrix row of the key
    report[2] = record->event.key.col;         // physical matrix column of the key
    report[3] = record->event.pressed ? 1 : 0; // 1 = key pressed, 0 = key released
    raw_hid_send(report, 32);                  // send the 32-byte HID report to the host app
    return true; // must return true so QMK continues normal key processing
}
```

> **Note:** If your keymap already defines `layer_state_set_user` or `process_record_user`, add this code into your existing function instead of making a duplicate. 

### 3. Enable Raw HID and VIA in rules.mk

In the same keymap folder (or your keyboard's `rules.mk` if you want it enabled by default), add:

```makefile
RAW_HID_ENABLE = yes
VIA_ENABLE = yes
```

### 4. Compile and flash the firmware

From the root of your `vial-qmk` directory, compile with:

```bash
qmk compile -kb <your_keyboard> -km <your_keymap>
```

For example:

```bash
qmk compile -kb ergohaven/imperial44/rev1 -km v1_v2
```

Then flash the resulted file onto your keyboard using `qmk flash` or use your board's usual flashing method

### 5.Installing dePressed
 
### Option A: Use the prebuilt executable
 
Prebuilt statically linked executables for Linux and Windows are provided in releases 
Widnwos and Linux binaries are staticly linked, so you are not required to have dependent libraries

On macOS, creating statically linked binaries is not natively supported. Therefore, please ensure you have the following libraries installed on your system before running the executable: `sdl3`, `sdl3-ttf`, `hidapi-hidraw`, and `liblzma` 

### Option B: Build from source
 
If you'd rather build it yourself or tinker with source code or a prebuilt binary isn't available for your platform, you'll need a C compiler and the following libraries installed: `sdl3`, `sdl3-ttf`, `hidapi-hidraw`, and `liblzma` 
 
From the root of the repo, compile with:
 
```bash
#Linux
gcc -g -I. main.c keyboard/hid.c GUI/font.c GUI/hit_test.c GUI/window.c GUI/button.c core/arena.c keyboard/init.c keyboard/vial.c keyboard/decode.c deps/cJSON.c GUI/error.c `pkg-config --static --cflags --libs sdl3 sdl3-ttf hidapi-hidraw liblzma` -lm -o depressed
```
 
This produces a `depressed` executable in the repo root. Run it with:
 
```bash
./depressed
```


### 6. Run dePressed

dePressed will automatically detect all of the suitable keyboards for you to choose

## Dependencies 

dePressed includes the following libs:

- SDL3 (zlib license) - https://libsdl.org
- liblzma / XZ Utils (0BSD) - https://tukaani.org/xz/
- hidapi (BSD-style license) - https://github.com/libusb/hidapic
- JSON (MIT license) - https://github.com/DaveGamble/cJSON

Their licenses are saved in licenses/

## TODO

Features that I want to implement in the future:

- Change UI colors
- Change window size
- ZMK support (do not have ZMK keyboard, so I cant actually test and debug)
- Raw QMK support
- Audio feedback for key presses with ability to choose audio file and turn it off
- Shortcut to hide an overlay without closing (need OS specific hooks)

