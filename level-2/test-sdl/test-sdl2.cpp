#ifdef _WIN32
#include <windows.h>
#endif

#include <SDL2/SDL.h>

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
// Force Window not to scale this application (DPI Awareness)
#ifdef _WIN32
    SetProcessDPIAware();
#endif

    // 1. Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // 2. Create Window
    SDL_Window* window = SDL_CreateWindow("SDL2 Graphics Test - UCRT64",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          800, 600,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // 3. Create Renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    std::cout << "SDL2 initialized successfully! Test starting..." << std::endl;

    // 4. Variables for Game Loop & FPS
    bool isRunning = true;
    SDL_Event event;

    Uint32 totalFrameCount = 0;  // Total frames since start
    Uint32 currentFPSCount = 0;  // Frames in the current second

    Uint32 startTime = SDL_GetTicks();  // Mark the start of the loop
    Uint32 lastTime = startTime;        // Used for per-second calculation

    std::cout << "Game Loop started. Close the window to exit." << std::endl;

    // --- GAME LOOP ---
    while (isRunning) {
        // 1. Event Handling
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

        // 2. Update Logic (Calculate FPS)
        totalFrameCount++;
        currentFPSCount++;

        Uint32 currentTime = SDL_GetTicks();

        // Update Real-time FPS every 1 second
        if (currentTime - lastTime >= 1000) {
            std::cout << "Current FPS: " << currentFPSCount << std::endl;
            currentFPSCount = 0;
            lastTime = currentTime;
        }

        // 3. Rendering
        SDL_SetRenderDrawColor(renderer, 20, 20, 70, 255);  // Dark Blue
        SDL_RenderClear(renderer);

        // Draw static shapes
        SDL_Rect fillRect = {200, 150, 400, 300};
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // Yellow
        SDL_RenderFillRect(renderer, &fillRect);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White Line
        SDL_RenderDrawLine(renderer, 0, 0, 800, 600);

        SDL_RenderPresent(renderer);
    }

    // 5. Post-Game Caculations
    Uint32 endTime = SDL_GetTicks();
    float totalSeconds = (endTime - startTime) / 1000.0f;

    std::cout << "------------------------------------" << std::endl;
    std::cout << "Loop Runtime: " << totalSeconds << " seconds." << std::endl;
    std::cout << "Total Frames: " << totalFrameCount << std::endl;

    if (totalSeconds > 0) {
        float avgFPS = totalFrameCount / totalSeconds;
        std::cout << "Average FPS: " << avgFPS << std::endl;
    }
    std::cout << "------------------------------------" << std::endl;

    // 6. Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    std::cout << "Test done!" << std::endl;

    return 0;
}
