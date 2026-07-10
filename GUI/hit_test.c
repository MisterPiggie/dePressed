#include "hit_test.h"
#include "core/num_types.h"
#include "button.h"

SDL_HitTestResult GUI_drag_and_resize(SDL_Window *window, const SDL_Point *point, void *data)
{
    App *app = (App *)data;
    S32 width, height;

    SDL_GetWindowSize(window, &width, &height);

    bool near_left = point->x < MARGIN;
    bool near_right = point->x >= width - MARGIN;
    bool near_top = point->y < MARGIN;
    bool near_bottom = point->y >= height - MARGIN;

    if (near_top && near_left)      
        return SDL_HITTEST_RESIZE_TOPLEFT;
    if (near_top && near_right)     
        return SDL_HITTEST_RESIZE_TOPRIGHT;
    if (near_bottom && near_left)   
        return SDL_HITTEST_RESIZE_BOTTOMLEFT;
    if (near_bottom && near_right)  
        return SDL_HITTEST_RESIZE_BOTTOMRIGHT;

    if (near_left)   
        return SDL_HITTEST_RESIZE_LEFT;
    if (near_right)  
        return SDL_HITTEST_RESIZE_RIGHT;
    if (near_top)    
        return SDL_HITTEST_RESIZE_TOP;
    if (near_bottom) 
        return SDL_HITTEST_RESIZE_BOTTOM;

    if (point_in_rect(point->x, point->y, &app->drag_button.rect))
        return SDL_HITTEST_DRAGGABLE;

    return SDL_HITTEST_NORMAL;
}
