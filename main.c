#include <hidapi/hidapi.h>
#include <unistd.h>
#include "core/types.h"
#include "GUI/window.h"
#include "GUI/button.h"
#include "GUI/font.h"
#include "keyboard/hid.h"
#include "keyboard/vial.h"
#include "keyboard/init.h"

int main(void)
{
    App app = {0};
    app.arena = arena_create(MB(100), KB(1));
    app.keyboards_count = 0;
    app.shared = arena_push_struct(&app.arena, App_shared);

    app.current_screen = SCREEN_SETUP;

    app.window = GUI_create_window();
    app.renderer = SDL_CreateRenderer(app.window, NULL);
    app.font = GUI_open_font();

    F32 text_width, text_height;
    SDL_GetTextureSize(texture, &text_width, &text_height);

    SDL_DestroySurface(surf);

    S32 win_width, win_height;

    SDL_GetWindowSize(window, &win_width, &win_height);


    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_RenderClear(renderer);

    SDL_Event event;
    app.running = 1;

    while (app.running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) 
                app.running = 0;

            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && 
                event.button.button == SDL_BUTTON_LEFT && button.is_hovered)
                button.is_pressed = 1;
            if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
                button.is_pressed = 0;
        }

        F32 mx, my;
        SDL_GetMouseState(&mx, &my);
        button.is_hovered = point_in_rect(mx, my, &button.rect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        if (button.is_pressed)
            SDL_SetRenderDrawColor(renderer, 45, 45, 45, 255);
        else if (button.is_hovered)
            SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        else 
            SDL_SetRenderDrawColor(renderer, 18, 18, 18, 255);

        SDL_RenderFillRect(renderer, &button.rect);
        SDL_RenderRect(renderer, &button.rect);

        SDL_FRect text = 
        {
            button.rect.x + (button.rect.w - text_width) / 2.0f,
            button.rect.y + (button.rect.h - text_height) / 2.0f,
            text_width, text_height
        };

        SDL_RenderTexture(renderer, texture, NULL, &text);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

    HID_get_suitable_keyboards(&app);
    if (app.keyboards_count == 0)
        //generate no keyboard found
        return 0;
    else if (app.keyboards_count == 1)
        app.active_model_idx = 0;
    else 
        //generate choose keyboard
        return 0;

    KBS_connect_keyboard(&app);

    listen_for_keypresses(&app.keyboards[app.active_model_idx], app.shared);

}

