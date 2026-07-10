#include <SDL3/SDL.h>
#include <stdbool.h>
#include "core/num_types.h"
#include "core/types.h"

#ifndef BUTTON_H
#define BUTTON_H


#define X_FRAC_OK  0.375f //37.5 from left
#define Y_FRAC_OK  0.45f  //45 from top
#define W_FRAC_OK  0.25f  //25 width
#define H_FRAC_OK  0.1f   //10 heigth

#define X_FRAC_DROP  0.375f
#define Y_FRAC_DROP  0.1f
#define W_FRAC_DROP  0.25f
#define H_FRAC_DROP  0.1f

#define X_FRAC_DRAG  0.8f
#define Y_FRAC_DRAG  0.2f
#define W_FRAC_DRAG  0.1f
#define H_FRAC_DRAG  0.1f

void init_buttons(App *app);
bool point_in_rect(F32 x, F32 y, const SDL_FRect *r);

#endif
