// main.cpp : Defines the entry point for the application.


#include <SDL3/SDL.h>

static bool running;

int main()
{

    if (!SDL_Init(SDL_INIT_VIDEO)) {

    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not Init SDL: %s\n",
                   SDL_GetError());
    }

    SDL_GL_LoadLibrary(NULL);


    SDL_Window *window;
    window = SDL_CreateWindow("Workshop", 800, 800, 0);

    if (window == NULL) {
      // In the case that the window could not be made...
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n",
                   SDL_GetError());
      return 1;
    }


    running = true;
    SDL_Event event;
    while (running)
    {
      while (SDL_PollEvent(&event)){
      }
     
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
