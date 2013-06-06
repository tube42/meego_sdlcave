#include <SDL/SDL.h>

#define WIDTH 840
#define HEIGHT 480


SDL_Surface *screen;

void render()
{
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0));
        SDL_Flip(screen);
}

void put_square(int x, int y)
{
        printf("%i, %i\n", x, y);
        SDL_Rect rect = { x - 5, y - 5, 10, 10};
        SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 0xFF, 0, 0));
        SDL_Flip(screen);
}

#undef main

int main(int argc, char **argv)
{
        SDL_Init(SDL_INIT_VIDEO);

        // screen = SDL_SetVideoMode(WIDTH, HEIGHT, 16, SDL_SWSURFACE);
        screen = SDL_SetVideoMode(0, 0, 16, SDL_SWSURFACE | SDL_FULLSCREEN);

        printf("Starting\n");

        render();

        SDL_Event event;
        while (SDL_WaitEvent(&event)) {
                switch (event.type) {
                        case SDL_QUIT:
                                goto quit;

                case SDL_MOUSEMOTION:
                        case SDL_MOUSEBUTTONDOWN:
                                put_square(event.button.x, event.button.y);
                                break;
                        case SDL_KEYDOWN:
                                switch (event.key.keysym.sym) {
                                        case SDLK_UP:
                                                SDL_WM_ToggleFullScreen(screen);
                                                break;
                                        case SDLK_LEFT:
                                                screen = SDL_SetVideoMode(WIDTH, HEIGHT, 16, SDL_SWSURFACE | SDL_FULLSCREEN);
                                                render();
                                                break;
                                        case SDLK_RIGHT:
                                                screen = SDL_SetVideoMode(WIDTH, HEIGHT, 16, SDL_SWSURFACE);
                                                render();
                                                break;
                                        case SDLK_DOWN:
                                                goto quit;
                                                break;
                                        default:
                                                break;
                                }
                                break;
                        case SDL_VIDEOEXPOSE:
                                printf("SDL_VIDEOExpose\n");
                                render();
                                break;
                }
        }

quit:
        printf("Quitting\n");
        SDL_Quit();

        return 0;
}
