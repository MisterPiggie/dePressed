#include "button.h"

bool point_in_rect(F32 x, F32 y, SDL_FRect *r)
{
    return (x >= r->x && x <= (r->x + r->w) && y >= r->y &&  y <= (r->y + r->h));
}

