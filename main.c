#include <stdio.h>
#include <SDL2/SDL.h>

#define WIDTH 900
#define HEIGHT 600

int main()
{
    printf("Some Instructions\n");
    printf("\n");
    printf("None Yet");
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("3d Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    SDL_UpdateWindowSurface(window);
    SDL_Delay(3000);
    return 0;
}
