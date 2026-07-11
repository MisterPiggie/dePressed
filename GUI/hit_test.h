#include <SDL3/SDL.h>
#include "core/types.h"


SDL_HitTestResult GUI_drag(SDL_Window *window, const SDL_Point *point, void *data);
void render_drag_handle(App *app);
