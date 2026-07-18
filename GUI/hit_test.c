#include "hit_test.h"
#include "core/num_types.h"
#include "button.h"

SDL_HitTestResult GUI_drag(SDL_Window *window, const SDL_Point *point, void *data)
{
    App *app = (App *)data;
    S32 width, height;

    SDL_GetWindowSize(window, &width, &height);

    if (point_in_rect(point->x, point->y, &app->drag_handle))
        return SDL_HITTEST_DRAGGABLE;

    return SDL_HITTEST_NORMAL;
}

void render_drag_handle(App *app)
{
    SDL_SetRenderDrawColor(app->renderer, app->idle_color.r, app->idle_color.g, app->idle_color.b, 255);

    F32 line_height = 2.0f;
    F32 gap = 9.0f;
    F32 inset = app->drag_handle.w * 0.1f; 

    F32 total_height = line_height * 3 + gap * 2;
    F32 start_y = app->drag_handle.y + (app->drag_handle.h - total_height) / 2.0f;

    for (int i = 0; i < 3; i++)
    {
        SDL_FRect line = {
            app->drag_handle.x + inset,
            app->drag_handle.y + i * (line_height + gap),
            app->drag_handle.w - inset * 2,
            line_height
        };
        SDL_RenderFillRect(app->renderer, &line);
    }
}



