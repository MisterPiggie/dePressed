#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>
#include <unistd.h>
#include <stdio.h>


typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
} Game;

int main(void)
{
    Game game;
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        puts("error init");
        return -1;
    }

    game.window = SDL_CreateWindow("test", 600, 200, SDL_WINDOW_TRANSPARENT);
    if (!game.window)
    {
        puts("error window");
        return -1;
    }
    SDL_SetWindowPosition(game.window, 0, 100);


    game.renderer = SDL_CreateRenderer(game.window, NULL);
    if (!game.renderer)
    {
        puts("error renderer");
        return -1;
    }

    SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 128);
    SDL_RenderClear(game.renderer);
    SDL_RenderPresent(game.renderer);

    SDL_Delay(5000);
    SDL_Quit();
    return 0;
}
