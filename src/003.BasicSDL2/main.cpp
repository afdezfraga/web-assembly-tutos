#include <iostream>
#include <SDL2/SDL.h>

#ifdef _EMSCRIPTEN
#include <emscripten.h>
#endif

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("Hello World", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Get the window surface
    SDL_Surface* screenSurface = SDL_GetWindowSurface(window);

    // Fill the surface with a green rectangle
    SDL_Rect rect = { (640 - 200) / 2, (480 - 100) / 2, 200, 100 }; // Centered rectangle
    SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0, 255, 0));

    // Update the surface
    SDL_UpdateWindowSurface(window);

    // Main loop flag
    bool quit = false;
    SDL_Event e;

    // Main loop
    #ifdef _EMSCRIPTEN
    emscripten_set_main_loop([]() {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                emscripten_cancel_main_loop();
            }
        }
    }, 0, 1);
    #endif
    #ifndef _EMSCRIPTEN
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
              quit = true;
            }

        }
    }
    #endif

    // Clean up and close the window
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}