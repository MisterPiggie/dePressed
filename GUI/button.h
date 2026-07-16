#ifndef BUTTON_H
#define BUTTON_H

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "core/num_types.h"
#include "core/types.h"



#define X_FRAC_OK  0.1f 
#define Y_FRAC_OK  0.3f  
#define W_FRAC_OK  0.3f  
#define H_FRAC_OK  0.1f  

#define X_FRAC_EXIT  0.1f 
#define Y_FRAC_EXIT  0.8f  
#define W_FRAC_EXIT  0.3f  
#define H_FRAC_EXIT  0.1f   

#define X_FRAC_RELOAD  0.1f
#define Y_FRAC_RELOAD 0.1f
#define W_FRAC_RELOAD 0.3f
#define H_FRAC_RELOAD 0.1f

#define X_FRAC_DROP  0.5f
#define Y_FRAC_DROP  0.1f
#define W_FRAC_DROP  0.4f
#define H_FRAC_DROP  0.1f

#define X_FRAC_DRAG  0.94f
#define Y_FRAC_DRAG  0.01f
#define W_FRAC_DRAG  0.05f
#define H_FRAC_DRAG  0.05f

void init_buttons(App *app);
bool point_in_rect(F32 x, F32 y, const SDL_FRect *r);
void draw_button(App *app, GUI_button *button);
void draw_dropdown(App *app, GUI_dropdown *dropdown);

#endif
