#include "error.h"
#include "font.h"


void GUI_create_an_error(App *app, const char *message)
{
    SDL_DestroyTexture(app->error_texture);
    app->error_texture = NULL;

    char error_message[128];
    sprintf(error_message, "An ERROR occured%s", message);

    app->error_texture = GUI_make_font_texture(app->font, app->renderer, error_message, app->fg_color);

    F32 text_width, text_height;
    S32 window_width, window_height;

    SDL_GetTextureSize(app->error_texture, &text_width, &text_height);
    SDL_GetWindowSize(app->window, &window_width, &window_height);

    app->error_rect = (SDL_FRect){
        (window_width - text_width) / 2.0f,
        window_height * 0.01,
        text_width,
        text_height
    };

    app->errored_at_time = SDL_GetTicks();
    app->is_errored = true;

    printf("ERROR CREATED\n");
}

void render_error(App *app)
{
    if (!app->is_errored)
        return;

    if (SDL_GetTicks() - app->errored_at_time > 5000)
        app->is_errored = false;
    
    SDL_RenderTexture(app->renderer, app->error_texture, NULL, &app->error_rect);
}
