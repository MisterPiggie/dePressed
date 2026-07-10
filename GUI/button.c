#include "button.h"
#include "font.h"

bool point_in_rect(F32 x, F32 y, const SDL_FRect *r)
{
    return (x >= r->x && x <= (r->x + r->w) && y >= r->y &&  y <= (r->y + r->h));
}

void init_buttons(App *app)
{
    S32 win_width, win_height;

    SDL_GetWindowSize(app->window, &win_width, &win_height);

    app->ok_button = (GUI_button)
    {
        .rect = 
        {
            .x = win_width * X_FRAC_OK,
            .y = win_width * Y_FRAC_OK,
            .w = win_width * W_FRAC_OK,
            .h = win_width * H_FRAC_OK,
        },
        .is_hovered = 0,
        .is_pressed = 0,
        .text_texture = GUI_make_font_texture(app->font, app->renderer, "OK", app->fg_color)
    };

    app->dropdown = (GUI_dropdown)
    {
        .rect = 
        {
            .x = win_width * X_FRAC_DROP,
            .y = win_width * Y_FRAC_DROP,
            .w = win_width * W_FRAC_DROP,
            .h = win_width * H_FRAC_DROP,
        },
        .is_open = 0,
    };

    app->drag_button = (GUI_button)
    {
        .rect = 
        {
            .x = win_width * X_FRAC_DRAG,
            .y = win_width * Y_FRAC_DRAG,
            .w = win_width * W_FRAC_DRAG,
            .h = win_width * H_FRAC_DRAG,
        },
        .is_hovered = 0,
        .is_pressed = 0,
        .text_texture = GUI_make_font_texture(app->font, app->renderer, "OK", app->fg_color)
    };
}



