// main.cpp : Defines the entry point for the application.


#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <iostream>

static bool running;

int main()
{

    if (!SDL_Init(SDL_INIT_VIDEO)) {

    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not Init SDL: %s\n",
                   SDL_GetError());
    }

    SDL_Window *window;
    window = SDL_CreateWindow("Workshop", 800, 800, SDL_WINDOW_OPENGL);

    if (window == NULL) {
      // In the case that the window could not be made...
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n",
                   SDL_GetError());
      return 1;
    }

    SDL_GLContext gl_context;
    gl_context = SDL_GL_CreateContext(window);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
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
      glViewport(0,0,800, 800);
      glClearColor(255, 0, 0, 0);
      glClear(GL_COLOR_BUFFER_BIT);
      SDL_GL_SwapWindow(window);
    }
    SDL_DestroyWindow(window);
    SDL_GL_DestroyContext(gl_context);


    SDL_Quit();
    return 0;
}
