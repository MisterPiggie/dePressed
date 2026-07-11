#include "font.h"


TTF_Font *GUI_open_font(void)
{
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("src/officecodepro-light.otf", 12);

    return font;
}

SDL_Texture *GUI_make_font_texture(TTF_Font *font, SDL_Renderer *renderer, const char *text, SDL_Color color)
{
    SDL_Surface *surf = TTF_RenderText_Blended(font, text, 0, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);

    SDL_DestroySurface(surf);

    return texture;
}

