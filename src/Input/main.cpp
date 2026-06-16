// main.cpp : Defines the entry point for the application.


#include <SDL3/SDL.h>

static bool running;

int main()
{

    if (!SDL_Init(SDL_INIT_VIDEO)) {

    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not Init SDL: %s\n",
                   SDL_GetError());
    }

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
        if (event.type == SDL_EVENT_QUIT) {
          running = false;
        }
        if (event.type == SDL_EVENT_MOUSE_MOTION) {
          SDL_Log("We got a motion event.");
          SDL_Log("Current mouse position is: (%f, %f)", event.motion.x,
                  event.motion.y);
        } else {
        SDL_Log("Unhandled Event!");
        }
      }
    }

    SDL_Quit();
    return 0;
}
