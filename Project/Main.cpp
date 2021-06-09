#include <iostream>
#include <chrono>
#include "Game.h"

int main(int argc, char* argv[])
{
    int result = Game::GetInstance().Init("GAME1017", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    if (result == 0)
    {
        // Allocate a time_point outside that loop so that it is retained between frames.
        auto lastFrameTime = std::chrono::high_resolution_clock::now();

        while (Game::GetInstance().IsRunning())
        {
            // Get current time_point.
            auto thisFrameTime = std::chrono::high_resolution_clock::now();

            // Subtract last frame's time_point from this frame's time_point to get the duration of the frame.
            std::chrono::duration<float> lastFrameDuration = thisFrameTime - lastFrameTime;

            // Convert the duration to a float.
            float deltaTime = lastFrameDuration.count();

            // Don't forget to pass newFrameCounter into lastFrameCounter.
            lastFrameTime = thisFrameTime;

            Game::GetInstance().HandleEvents();
            Game::GetInstance().Update(deltaTime);
            Game::GetInstance().Render();

            //std::cout << " hit" << std::endl;
        }
        Game::GetInstance().Clean();
    }

    return result;
}


