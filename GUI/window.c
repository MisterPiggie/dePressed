#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "window.h"
#include "button.h"
#include "keyboard/hid.h"
#include "hit_test.h"
#include "core/num_types.h"
#include "keyboard/init.h"

SDL_Window *GUI_create_window(void)
{
    SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "x11");
    SDL_Init(SDL_INIT_VIDEO);

    SDL_DisplayID display_id = SDL_GetPrimaryDisplay();
    const SDL_DisplayMode *mode = SDL_GetDesktopDisplayMode(display_id);

    U32 screen_width = mode->w;
    U32 screen_height = mode->h;

    F32 scale_width = 0.5f;
    F32 scale_height = 0.3f;

    U32 window_width = (U32)screen_width * scale_width;
    U32 window_height = (U32)screen_height * scale_height;

    
    
    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, "dePressed");
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

void handle_setup_events(App *app, SDL_Event *event)
{
    F32 mouse_x = event->button.x;
    F32 mouse_y = event->button.y;
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && event->button.button == SDL_BUTTON_LEFT)
    {
        if (app->dropdown.is_open)
        {
            for (int i = 0; i < app->keyboards_count; i++)
            {
                if (point_in_rect(mouse_x, mouse_y, &app->dropdown.link[i].rect))
                    app->dropdown.selected_idx = app->dropdown.link[i].idx;
            }
            app->dropdown.is_open = false;
        }
        if (point_in_rect(mouse_x, mouse_y, &app->ok_button.rect))
            app->ok_button.is_pressed = true;
        if (point_in_rect(mouse_x, mouse_y, &app->exit_button.rect))
            app->exit_button.is_pressed = true;
        if (point_in_rect(mouse_x, mouse_y, &app->reload_button.rect))
            app->reload_button.is_pressed = true;
        if (point_in_rect(mouse_x, mouse_y, &app->dropdown.rect))
            app->dropdown.is_pressed = true;

    }

    if (event->type == SDL_EVENT_MOUSE_BUTTON_UP && event->button.button == SDL_BUTTON_LEFT)
    {
        if (point_in_rect(mouse_x, mouse_y, &app->ok_button.rect))
        {
            if (app->dropdown.selected_idx >= 0)
            {
                if (app->active_model_idx != app->dropdown.selected_idx)
                {
                    app->active_model_idx = app->dropdown.selected_idx;
                    if (!KBS_connect_keyboard(app))
                    {
                        if (app->active_model_idx != -1)
                            KBS_disconnect_keyboard(app);

                        arena_rewind(&app->arena);
                        app->keyboards_count = 0;
                        app->active_model_idx = -1;
                        HID_get_suitable_keyboards(app);
                    }
                } 
                if (app->active_model_idx != -1)
                    app->current_screen = SCREEN_MAIN;
            }
        }

        app->ok_button.is_pressed = false;

        if (point_in_rect(mouse_x, mouse_y, &app->exit_button.rect))
        {
            SDL_Event quit_event;
            quit_event.type = SDL_EVENT_QUIT;
            SDL_PushEvent(&quit_event);
        }
        app->exit_button.is_pressed = false;

        if (point_in_rect(mouse_x, mouse_y, &app->reload_button.rect))
        {
            if (app->active_model_idx != -1)
                KBS_disconnect_keyboard(app);

            arena_rewind(&app->arena);
            app->keyboards_count = 0;
            app->active_model_idx = -1;
            HID_get_suitable_keyboards(app);
        }
        app->reload_button.is_pressed = false;

        if (point_in_rect(mouse_x, mouse_y, &app->dropdown.rect))
        {
            app->dropdown.is_open = true;
        }
        app->dropdown.is_pressed = false;
    }

    if (event->type == SDL_EVENT_MOUSE_MOTION)
    {
        if (app->dropdown.is_open)
            for (int i = 0; i < app->keyboards_count; i++)
                app->dropdown.link[i].is_hovered = point_in_rect(mouse_x, mouse_y, &app->dropdown.link[i].rect);

        app->ok_button.is_hovered = point_in_rect(mouse_x, mouse_y, &app->ok_button.rect);
        app->exit_button.is_hovered = point_in_rect(mouse_x, mouse_y, &app->exit_button.rect);
        app->reload_button.is_hovered = point_in_rect(mouse_x, mouse_y, &app->reload_button.rect);
        app->dropdown.is_hovered = point_in_rect(mouse_x, mouse_y, &app->dropdown.rect);
    }
}

void handle_main_events(App *app, SDL_Event *event)
{
    (void) app;
    (void) event;
}

void handle_event(App *app, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        app->running = 0;
        return;
    }

    if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_ESCAPE)
    {
        if (app->current_screen == SCREEN_MAIN)
            app->current_screen = SCREEN_SETUP;
        else 
        {
            if (app->dropdown.selected_idx >= 0)
            {
                if (app->active_model_idx != app->dropdown.selected_idx)
                {
                    app->active_model_idx = app->dropdown.selected_idx;
                    KBS_connect_keyboard(app);
                } 
                app->current_screen = SCREEN_MAIN;
            }
        }

        return;
    }

    switch (app->current_screen)
    {
        case SCREEN_SETUP:
            handle_setup_events(app, event);
            break;
        case SCREEN_MAIN:
            handle_main_events(app, event);
            break;
    }
}

void render_setup_screen(App *app)
{
    draw_dropdown(app, &app->dropdown);
    draw_button(app, &app->ok_button);
    draw_button(app, &app->exit_button);
    draw_button(app, &app->reload_button);
}

void render_main_screen(App *app)
{
    if (app->keyboards_count == 0)
        return;

    KBS_model *model = &app->keyboards[app->active_model_idx];  

    for (int i = 0; i < model->layout.key_count; i++)
    {
        KBS_key *key = &model->layout.keys[i];  

        SDL_Texture *bg_texture = app->shared.pressed[i] ? key->pressed_texture : key->idle_texture;
        if (app->shared.pressed[i])
        {
            printf("Key row: %d col: %d\nKey codes:\n", key->row, key->col);
            for (int j = 0; j < model->layers_count; j++)
                printf("%04x\n ", key->code[j]);
            printf("\n");
        }
        SDL_RenderTextureRotated(app->renderer, bg_texture, NULL, &key->rect, key->angle, &key->center, SDL_FLIP_NONE);

        S8 layer_idx = KBS_resolve_layer(key, app->shared.active_layers);

        SDL_RenderTextureRotated(app->renderer, key->code_textures[layer_idx], NULL, &key->code_text_rects[layer_idx], key->angle, &key->center, SDL_FLIP_NONE);
    }
}

void render_frame(App *app)
{
    SDL_SetRenderDrawColor(app->renderer, app->bg_color.r, app->bg_color.g, app->bg_color.b, 255);
    SDL_RenderClear(app->renderer);
 
    switch (app->current_screen)
    {
        case SCREEN_SETUP: 
            render_setup_screen(app); 
            break;
        case SCREEN_MAIN: 
            render_main_screen(app);  
            break;
    }
 
    render_drag_handle(app);    
 
    SDL_RenderPresent(app->renderer);
}

