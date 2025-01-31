#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        cerr << "SDL_Init Error: " << SDL_GetError() << endl;
        return 1;
    }

    // Create a window
    SDL_Window *window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window)
    {
        cerr << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Paddles and ball
    SDL_Rect leftPaddle = {50, SCREEN_HEIGHT / 2 - 50, 20, 100};
    SDL_Rect rightPaddle = {SCREEN_WIDTH - 70, SCREEN_HEIGHT / 2 - 50, 20, 100};
    SDL_Rect ball = {SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 - 10, 20, 20};
    int ballSpeedX = 5, ballSpeedY = 5;
    int leftScore = 0, rightScore = 0;

    // Game loop
    bool running = true;
    while (running)
    {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        // Handle input
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_W] && leftPaddle.y > 0)
            leftPaddle.y -= 5;
        if (keys[SDL_SCANCODE_S] && leftPaddle.y < SCREEN_HEIGHT - leftPaddle.h)
            leftPaddle.y += 5;
        if (keys[SDL_SCANCODE_UP] && rightPaddle.y > 0)
            rightPaddle.y -= 5;
        if (keys[SDL_SCANCODE_DOWN] && rightPaddle.y < SCREEN_HEIGHT - rightPaddle.h)
            rightPaddle.y += 5;

        // Update ball position
        ball.x += ballSpeedX;
        ball.y += ballSpeedY;

        // Bounce off walls
        if (ball.y <= 0 || ball.y >= SCREEN_HEIGHT - ball.h)
            ballSpeedY = -ballSpeedY;

        // Bounce off paddles
        if (SDL_HasIntersection(&ball, &leftPaddle) || SDL_HasIntersection(&ball, &rightPaddle))
        {
            ballSpeedX = -ballSpeedX;
        }

        // Reset ball and update score
        if (ball.x <= 0)
        {
            rightScore++;
            ball.x = SCREEN_WIDTH / 2 - 10;
            ball.y = SCREEN_HEIGHT / 2 - 10;
        }
        if (ball.x >= SCREEN_WIDTH - ball.w)
        {
            leftScore++;
            ball.x = SCREEN_WIDTH / 2 - 10;
            ball.y = SCREEN_HEIGHT / 2 - 10;
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw paddles and ball
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &leftPaddle);
        SDL_RenderFillRect(renderer, &rightPaddle);
        SDL_RenderFillRect(renderer, &ball);

        // Update the screen
        SDL_RenderPresent(renderer);

        // Cap the frame rate
        SDL_Delay(16); // ~60 FPS
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}