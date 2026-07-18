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
        .win_height = win_height,
        .win_width = win_width,
        .is_open = 0,
        .selected_idx = -1
    };
    app->dropdown.placeholder_texture = GUI_make_font_texture(app->font, app->renderer, "--No keyboards were found--", app->fg_color);


    app->drag_handle = (SDL_FRect)
    {
        .x = win_width * X_FRAC_DRAG,
        .y = win_width * Y_FRAC_DRAG,
        .w = win_width * W_FRAC_DRAG,
        .h = win_width * H_FRAC_DRAG,
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
    SDL_Color bg = dropdown->is_pressed ? app->pressed_color
                 : dropdown->is_hovered ? app->hover_color : app->idle_color;

    SDL_SetRenderDrawColor(app->renderer, bg.r, bg.g, bg.b, bg.a);
    SDL_RenderFillRect(app->renderer, &dropdown->rect);

    SDL_Texture *texture = (dropdown->selected_idx == -1) ? dropdown->placeholder_texture : dropdown->options_texture[dropdown->selected_idx];

    F32 text_width, text_height;
    SDL_GetTextureSize(texture, &text_width, &text_height);

    SDL_FRect text_dest =
    {
        dropdown->rect.x + (dropdown->rect.w - text_width) / 2.0f,
        dropdown->rect.y + (dropdown->rect.h - text_height) / 2.0f,
        text_width,
        text_height,
    };

    if (dropdown->is_open)
    {

        for (int i = 0; i < app->keyboards_count; i++)
        {
            GUI_dropdown_link *link = &dropdown->link[i];
            SDL_Color bg = link->is_hovered ? app->hover_color : app->idle_color;

            SDL_SetRenderDrawColor(app->renderer, bg.r, bg.g, bg.b, bg.a);
            SDL_RenderFillRect(app->renderer, &link->rect);

            if (dropdown->options_texture[i])
            {
                F32 text_width;
                F32 text_height;

                SDL_GetTextureSize(dropdown->options_texture[i], &text_width, &text_height);

                SDL_FRect text_dest =
                {
                    link->rect.x + (link->rect.w - text_width) / 2.0f,
                    link->rect.y + (link->rect.h - text_height) / 2.0f,
                    text_width,
                    text_height,
                };

                SDL_RenderTexture(app->renderer, dropdown->options_texture[i], NULL, &text_dest);
            }
        }
    }
    SDL_RenderTexture(app->renderer, texture, NULL, &text_dest);
}
