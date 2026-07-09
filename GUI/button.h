#include <SDL3/SDL.h>
#include <stdbool.h>
#include "num_types.h"

#define X_FRAC  0.375f
#define Y_FRAC  0.45f
#define W_FRAC  0.25f
#define H_FRAC  0.1f

typedef struct 
{
    SDL_FRect   rect;
    const char  *label;

    bool        is_hovered;
    bool        is_pressed;
} GUI_button;

bool point_in_rect(F32 x, F32 y, SDL_FRect *r);
