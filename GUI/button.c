#include "button.h"
#include "font.h"
#include <SDL3_image/SDL_image.h>


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
            .y = win_height * Y_FRAC_OK,
            .w = win_width * W_FRAC_OK,
            .h = win_height * H_FRAC_OK,
        },
        .is_hovered = 0,
        .is_pressed = 0,
        .text_texture = GUI_make_font_texture(app->font, app->renderer, "OK", app->fg_color)
    };

    app->reload_button = (GUI_button)
    {
        .rect = 
        {
            .x = win_width * X_FRAC_RELOAD,
            .y = win_height * Y_FRAC_RELOAD,
            .w = win_width * W_FRAC_RELOAD,
            .h = win_height * H_FRAC_RELOAD,
        },
        .is_hovered = 0,
        .is_pressed = 0,
        .text_texture = GUI_make_font_texture(app->font, app->renderer, "RELOAD", app->fg_color)
    };

    app->exit_button = (GUI_button)
    {
        .rect = 
        {
            .x = win_width * X_FRAC_EXIT,
            .y = win_height * Y_FRAC_EXIT,
            .w = win_width * W_FRAC_EXIT,
            .h = win_height * H_FRAC_EXIT,
        },
        .is_hovered = 0,
        .is_pressed = 0,
        .text_texture = GUI_make_font_texture(app->font, app->renderer, "EXIT", app->fg_color)
    };

    app->dropdown = (GUI_dropdown)
    {
        .rect = 
        {
            .x = win_width * X_FRAC_DROP,
            .y = win_height * Y_FRAC_DROP,
            .w = win_width * W_FRAC_DROP,
            .h = win_height * H_FRAC_DROP,
        },
        .is_open = 0,
        .selected_idx = -1

    };

    if (app->keyboards_count == 0)
        app->dropdown.placeholder_texture = GUI_make_font_texture(app->font, app->renderer, "--No keyboards were found--", app->fg_color);
    else if (app->keyboards_count > 1)
        app->dropdown.placeholder_texture = GUI_make_font_texture(app->font, app->renderer, "--Choose your keyboard--", app->fg_color);
    else 
        app->dropdown.selected_idx = 0;

    app->dropdown.options_texture = arena_push_array(&app->arena, SDL_Texture *, app->keyboards_count);

    for (int i = 0; i < app->keyboards_count; i++)
        app->dropdown.options_texture[i] = GUI_make_font_texture(app->font, app->renderer, app->keyboards[i].product_name, app->fg_color);

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
        .text_texture = IMG_LoadTexture(app->renderer, "src/drag-hand-gesture-svgrepo-com.svg")
    };
}

void draw_button(App *app, GUI_button *button)
{
    SDL_Color bg = button->is_pressed ? app->pressed_color
                 : button->is_hovered ? app->hover_color : app->idle_color;
 
    SDL_SetRenderDrawColor(app->renderer, bg.r, bg.g, bg.b, bg.a);
    SDL_RenderFillRect(app->renderer, &button->rect);

    if (button->text_texture)
    {
        F32 text_width;
        F32 text_height;

        SDL_GetTextureSize(button->text_texture, &text_width, &text_height);

        SDL_FRect text_dest =
        {
            button->rect.x + (button->rect.w - text_width) / 2.0f,
            button->rect.y + (button->rect.h - text_height) / 2.0f,
            text_width,
            text_height,
        };

        SDL_RenderTexture(app->renderer, button->text_texture, NULL, &text_dest);
    }
}

void draw_dropdown(App *app, GUI_dropdown *dropdown)
{
    SDL_SetRenderDrawColor(app->renderer, app->idle_color.r, app->idle_color.g, app->idle_color.b, app->idle_color.a);
    SDL_RenderFillRect(app->renderer, &dropdown->rect);

    if (dropdown->selected_idx == -1)
    {
        F32 text_width;
        F32 text_height;

        SDL_GetTextureSize(dropdown->placeholder_texture, &text_width, &text_height);

        SDL_FRect text_dest =
        {
            dropdown->rect.x + (dropdown->rect.w - text_width) / 2.0f,
            dropdown->rect.y + (dropdown->rect.h - text_height) / 2.0f,
            text_width,
            text_height,
        };

        SDL_RenderTexture(app->renderer, dropdown->placeholder_texture, NULL, &text_dest);
    }
}
 



