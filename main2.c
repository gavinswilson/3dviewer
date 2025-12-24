#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int clamp360(int v)
{
    if (v == -1) return 359;
    if (v == 360) return 0;
    return v;
}



void drawAxes(SDL_Renderer *renderer, int cx, int cy, int size,
              int rx, int ry, int rz)
{
    // Convert degrees to radians
    float ax = rx * M_PI / 180.0f;
    float ay = ry * M_PI / 180.0f;
    float az = rz * M_PI / 180.0f;

    // Axis unit vectors
    float axes[3][3] = {
        {1, 0, 0}, // X
        {0, 1, 0}, // Y
        {0, 0, 1}  // Z
    };

    for (int i = 0; i < 3; i++) {
        float x = axes[i][0];
        float y = axes[i][1];
        float z = axes[i][2];

        // Rotate around X
        float y1 = y * cosf(ax) - z * sinf(ax);
        float z1 = y * sinf(ax) + z * cosf(ax);

        // Rotate around Y
        float x2 = x * cosf(ay) + z1 * sinf(ay);
        float z2 = -x * sinf(ay) + z1 * cosf(ay);

        // Rotate around Z
        float x3 = x2 * cosf(az) - y1 * sinf(az);
        float y3 = x2 * sinf(az) + y1 * cosf(az);

        // Simple orthographic projection
        int xEnd = cx + (int)(x3 * size);
        int yEnd = cy - (int)(y3 * size);

        // Color per axis (right-handed)
        if (i == 0) SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);   // X
        if (i == 1) SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);   // Y
        if (i == 2) SDL_SetRenderDrawColor(renderer, 0, 128, 255, 255); // Z

        SDL_RenderDrawLine(renderer, cx, cy, xEnd, yEnd);
    }
}

void drawSun(SDL_Renderer *renderer, int cx, int cy, int size)
{
    filledCircleRGBA(renderer, cx, cy, size, 255, 255, 0, 255);
    // SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    // SDL_Rect square = { cx, cy, size, size }; // x, y, w, h
    // SDL_RenderFillRect(renderer, &square);
}


void drawEllipse(SDL_Renderer *renderer, int cx, int cy, int rx, int ry)
{
    ellipseRGBA(renderer, cx, cy, rx, ry, 255, 255, 0, 255);
}
void updateText(SDL_Renderer *renderer, TTF_Font *font,
                SDL_Texture **texture, SDL_Rect *rect,
                const char *text, SDL_Color color,
                int x, int y)
{
    if (*texture)
        SDL_DestroyTexture(*texture);

    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
    *texture = SDL_CreateTextureFromSurface(renderer, surface);

    rect->x = x;
    rect->y = y;
    rect->w = surface->w;
    rect->h = surface->h;

    SDL_FreeSurface(surface);
}

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

    font = TTF_OpenFont("DejaVuSans.ttf", 18);
    if (!font) {
        printf("Font error: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color red = {255, 0, 0, 255};
    SDL_Color green = {0, 255, 0, 255};
    SDL_Color blue = {0, 0, 255, 255};
    SDL_Color black = {0, 0, 0, 255};
    SDL_Color yellow = {255, 255, 0, 255};
    int baseY = WINDOW_HEIGHT - 80;

    // Initial text creation
    char buf[32];
    snprintf(buf, sizeof(buf), "x: %d", x);
    updateText(renderer, font, &xTex, &xRect, buf, red, 10, baseY);

    snprintf(buf, sizeof(buf), "y: %d", y);
    updateText(renderer, font, &yTex, &yRect, buf, green, 10, baseY + 25);

    snprintf(buf, sizeof(buf), "z: %d", z);
    updateText(renderer, font, &zTex, &zRect, buf, blue, 10, baseY + 50);

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

                if (changed) {
                    x = clamp360(x);
                    y = clamp360(y);
                    z = clamp360(z);

                    snprintf(buf, sizeof(buf), "x: %d", x);
                    updateText(renderer, font, &xTex, &xRect, buf, white, 10, baseY);

                    snprintf(buf, sizeof(buf), "y: %d", y);
                    updateText(renderer, font, &yTex, &yRect, buf, white, 10, baseY + 25);

                    snprintf(buf, sizeof(buf), "z: %d", z);
                    updateText(renderer, font, &zTex, &zRect, buf, white, 10, baseY + 50);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, xTex, NULL, &xRect);
        SDL_RenderCopy(renderer, yTex, NULL, &yRect);
        SDL_RenderCopy(renderer, zTex, NULL, &zRect);
        
        drawAxes(renderer,
                120, WINDOW_HEIGHT - 40,  // center position
                40,                       // axis length
                x, y, z);                 // rotation values
        drawSun(renderer, WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 10);
        drawEllipse(renderer, WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 100, 50);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyTexture(xTex);
    SDL_DestroyTexture(yTex);
    SDL_DestroyTexture(zTex);

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();
    return 0;
}
