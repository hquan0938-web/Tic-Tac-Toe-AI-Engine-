#include <SDL3/SDL.h>

#include <iostream>

int main(int argc, char* argv[]) {
    // 1. Initialize SDL3
    // SDL3 returns true on success, false on failure
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL3 could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // 2. Create Window
    // High-DPI is enabled by default in SDL3
    SDL_Window* window = SDL_CreateWindow("SDL3 Graphics Test - UCRT64", 800, 600, 0);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // 3. Create Renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    std::cout << "SDL3 initialized successfully! Test starting..." << std::endl;

    // 4. Variables for Game Loop & FPS
    bool isRunning = true;
    SDL_Event event;

    Uint64 totalFrameCount = 0;  // Total frames since start
    Uint32 currentFPSCount = 0;  // Frames in the current second

    Uint64 startTime = SDL_GetTicks();  // Mark the start of the loop
    Uint64 lastTime = startTime;        // Used for per-second calculation

    std::cout << "Game Loop started. Close the window to exit." << std::endl;

    // --- GAME LOOP ---
    while (isRunning) {
        // 1. Event Handling
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {  // SDL3 uses SDL_EVENT_QUIT
                isRunning = false;
            }
        }

        // 2. Update Logic (Calculate FPS)
        totalFrameCount++;
        currentFPSCount++;

        Uint64 currentTime = SDL_GetTicks();

        // Update Real-time FPS every 1 second
        if (currentTime - lastTime >= 1000) {
            std::cout << "Current FPS: " << currentFPSCount << std::endl;
            currentFPSCount = 0;
            lastTime = currentTime;
        }

        // 3. Rendering
        // A. Clear screen with a Dark Grey background
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_RenderClear(renderer);

        // B. Draw a Green Rectangle (Filled)
        SDL_FRect fillRect = {250.0f, 200.0f, 300.0f, 200.0f};
        SDL_SetRenderDrawColor(renderer, 0, 255, 100, 255);  // Neon Green
        SDL_RenderFillRect(renderer, &fillRect);

        // C. Draw a Cyan Outline Rectangle
        SDL_FRect outlineRect = {230.0f, 180.0f, 340.0f, 240.0f};
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);  // Cyan
        SDL_RenderRect(renderer, &outlineRect);

        // D. Draw multiple Diagonal Lines
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // Yellow
        SDL_RenderLine(renderer, 0, 0, 800, 600);
        SDL_RenderLine(renderer, 800, 0, 0, 600);

        // Update Screen
        SDL_RenderPresent(renderer);
    }

    // 5. Post-Game Calculations
    Uint64 endTime = SDL_GetTicks();
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
