#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "window.h"
#include "button.h"
#include "num_types.h"

int main(void)
{

    SDL_Window *window = GUI_create_window();
    TTF_Init();

    TTF_Font *font = TTF_OpenFont("officecodepro-light.otf", 12);
    SDL_Color white = {255, 255, 255, 255};

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    SDL_Surface *surf = TTF_RenderText_Blended(font, "OK", 0, white);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
    F32 text_width, text_height;
    SDL_GetTextureSize(texture, &text_width, &text_height);

    SDL_DestroySurface(surf);

    S32 win_width, win_height;

    SDL_GetWindowSize(window, &win_width, &win_height);

    GUI_button button = 
    {
        .rect = 
        {
            .x = win_width * X_FRAC,
            .y = win_width * Y_FRAC,
            .w = win_width * W_FRAC,
            .h = win_width * H_FRAC,
        },
        .is_hovered = 0,
        .is_pressed = 0
    };

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_RenderClear(renderer);

    SDL_Event event;
    U8 running = 1;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = 0;

            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && 
                event.button.button == SDL_BUTTON_LEFT &&
                button.is_hovered)
            {
                button.is_pressed = 1;
            }
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

SDL_Window *GUI_create_window(void)
{
    SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "x11");
    SDL_Init(SDL_INIT_VIDEO);

    SDL_DisplayID display_id = SDL_GetPrimaryDisplay();
    const SDL_DisplayMode *mode = SDL_GetDesktopDisplayMode(display_id);

    U32 screen_width = mode->w;
    U32 screen_height = mode->h;

    F32 scale = 0.3f;

    U32 window_width = (U32)screen_width * scale;
    U32 window_height = (U32)screen_height * scale;

    
    
    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X_NUMBER, 10);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, 10);

    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, window_width);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, window_height);

    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN, true);
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_TRANSPARENT_BOOLEAN, true);
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_ALWAYS_ON_TOP_BOOLEAN, true);

    SDL_Window *window = SDL_CreateWindowWithProperties(props);
    SDL_DestroyProperties(props);

    return window;
}
