#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600





int main(int argc, char *argv[])
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;

    SDL_Texture *xTex = NULL;
    SDL_Texture *yTex = NULL;
    SDL_Texture *zTex = NULL;

    SDL_Rect xRect, yRect, zRect;

    int x = 0, y = 0, z = 0;
    int running = 1;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    window = SDL_CreateWindow(
        "3d Viewer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;

            if (event.type == SDL_KEYDOWN) {
                int changed = 0;

                switch (event.key.keysym.sym) {
                    case SDLK_q: x--; changed = 1; break;
                    case SDLK_w: x++; changed = 1; break;
                    case SDLK_a: y--; changed = 1; break;
                    case SDLK_s: y++; changed = 1; break;
                    case SDLK_z: z--; changed = 1; break;
                    case SDLK_x: z++; changed = 1; break;
                }

            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        const SDL_Rect rect = { x, y, 50, 50 };

        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // yellow
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();
    return 0;
}
