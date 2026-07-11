#include "hit_test.h"
#include "core/num_types.h"
#include "button.h"

SDL_HitTestResult GUI_drag(SDL_Window *window, const SDL_Point *point, void *data)
{
    App *app = (App *)data;
    S32 width, height;

    SDL_GetWindowSize(window, &width, &height);

    if (point_in_rect(point->x, point->y, &app->drag_button.rect))
        return SDL_HITTEST_DRAGGABLE;

    return SDL_HITTEST_NORMAL;
}

void render_drag_handle(App *app)
{
    GUI_button drag = app->drag_button;
    F32 img_width, img_height;
    SDL_GetTextureSize(drag.text_texture, &img_width, &img_height);
    SDL_FRect img_dest =
    {
        drag.rect.x,
        drag.rect.y,
        drag.rect.w,
        drag.rect.h,
    };

    SDL_RenderTexture(app->renderer, drag.text_texture, NULL, &img_dest);
}
