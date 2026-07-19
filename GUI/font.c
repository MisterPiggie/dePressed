#include <SDL3/SDL.h>
#include "font.h"
#include "JBM.h"


TTF_Font *GUI_open_font(void)
{
    TTF_Init();
    SDL_IOStream *io_stream = SDL_IOFromConstMem( src_JetBrainsMonoNL_Bold_ttf, (int)src_JetBrainsMonoNL_Bold_ttf_len);
    TTF_Font *font = TTF_OpenFontIO(io_stream, true, 13);

    return font;
}

SDL_Texture *GUI_make_font_texture(TTF_Font *font, SDL_Renderer *renderer, const char *text, SDL_Color color)
{
    SDL_Surface *surf = TTF_RenderText_Blended(font, text, 0, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);

    SDL_DestroySurface(surf);

    return texture;
}

