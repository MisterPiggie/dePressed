#include <hidapi/hidapi.h>
#include <unistd.h>
#include "core/types.h"
#include "GUI/window.h"
#include "GUI/button.h"
#include "GUI/hit_test.h"
#include "GUI/font.h"
#include "keyboard/hid.h"
#include "keyboard/vial.h"
#include "keyboard/init.h"

int main(void)
{
    App app = {0};
    app.arena = arena_create(MB(100), KB(1));
    app.keyboards_count = 0;
    app.active_model_idx = -1;
    app.shared = arena_push_struct(&app.arena, App_shared);

    app.current_screen = SCREEN_SETUP;

    app.window = GUI_create_window();
    app.renderer = SDL_CreateRenderer(app.window, NULL);

    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_BLEND);

    app.font = GUI_open_font();

    app.bg_color = (SDL_Color){ 38, 31, 50, 255 };
    app.fg_color = (SDL_Color){ 255, 255, 255, 255 };
    app.idle_color = (SDL_Color){ 65, 57, 79, 255 };
    app.hover_color = (SDL_Color){ 98, 93, 115, 255 };
    app.pressed_color = (SDL_Color){ 89, 75, 160, 255 };

    HID_get_suitable_keyboards(&app);

    init_buttons(&app);

    SDL_SetWindowHitTest(app.window, GUI_drag, &app);

    app.running = 1;

    while (app.running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
            handle_event(&app, &e);  

        render_frame(&app);          
    }
}

