#include "prog.h"
#include <time.h>
#include <SDL2/SDL_ttf.h>
#include <emscripten/emscripten.h>

struct Prog *p;
void emloop(void *arg)
{
    bool restart = prog_mainloop(p);

    if (!p->running && !restart)
        emscripten_cancel_main_loop();
    if (!p->running && restart)
    {
        SDL_Window *w = p->window;
        SDL_Renderer *r = p->rend;
        prog_free(p);
        p = prog_alloc(w, r);
    }
}

int main(int argc, char **argv)
{
    srand(time(0));

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Window *w = SDL_CreateWindow("Spherefield", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 400, 400, SDL_WINDOW_SHOWN);
    SDL_Renderer *r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    p = prog_alloc(w, r);
    emscripten_set_main_loop_arg(emloop, p, -1, 1);
    /* bool restart = prog_mainloop(p); */
    /* prog_free(p); */

    /* if (!restart) */
    /*     break; */

    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);

    TTF_Quit();
    SDL_Quit();

    return 0;
}

