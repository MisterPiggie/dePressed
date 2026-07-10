#include <SDL3_ttf/SDL_ttf.h>

TTF_Font *GUI_open_font(void);
SDL_Texture *GUI_make_font_texture(TTF_Font *font, SDL_Renderer *renderer, const char *text, SDL_Color color);
