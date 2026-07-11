#ifndef WINDOW_H
#define WINDOW_H
#include <SDL3/SDL.h>
#include "core/types.h"


SDL_Window *GUI_create_window(void);
void handle_setup_events(App *app, SDL_Event *e);
void handle_event(App *app, SDL_Event *event);
void render_frame(App *app);

#endif
