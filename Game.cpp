#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

// Constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PIPE_GAP = 150;
const int PIPE_WIDTH = 50;
const int BIRD_WIDTH = 40;
const int BIRD_HEIGHT = 30;
const int GRAVITY = 1;

// Structs
struct Bird {
    int x;
    int y;
    int velocity;
};

struct Pipe {
    int x;
    int height;
    bool counted;
};

// Global variables
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Texture* gBackgroundTexture = nullptr;
SDL_Texture* gBirdTexture = nullptr;
SDL_Texture* gPipeTexture = nullptr;

// Function prototypes
bool init();
bool loadMedia();
void close();
void update(Bird& bird, std::vector<Pipe>& pipes);
void render(Bird& bird, std::vector<Pipe>& pipes);
int random(int min, int max);

// Function implementations
bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
    }

    gWindow = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == nullptr) {
        std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == nullptr) {
        std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    return true;
}

bool loadMedia() {
    SDL_Surface* surface = nullptr;

    surface = IMG_Load("background.png");
    if (surface == nullptr) {
        std::cout << "Unable to load image! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }
    gBackgroundTexture = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("bird.png");
    if (surface == nullptr) {
        std::cout << "Unable to load image! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }
    gBirdTexture = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("pipe.png");
    if (surface == nullptr) {
        std::cout << "Unable to load image! SDL_image Error: " << IMG_GetError() << std::endl;

}
gPipeTexture = SDL_CreateTextureFromSurface(gRenderer, surface);
SDL_FreeSurface(surface);

return true;
}

void close() {
SDL_DestroyTexture(gBackgroundTexture);
SDL_DestroyTexture(gBirdTexture);
SDL_DestroyTexture(gPipeTexture);
gBackgroundTexture = nullptr;
gBirdTexture = nullptr;
gPipeTexture = nullptr;
SDL_DestroyRenderer(gRenderer);
SDL_DestroyWindow(gWindow);
gRenderer = nullptr;
gWindow = nullptr;

IMG_Quit();
SDL_Quit();
}

void update(Bird& bird, std::vector<Pipe>& pipes) {
// Move bird
bird.velocity += GRAVITY;
bird.y += bird.velocity;
// Move pipes
for (auto& pipe : pipes) {
    pipe.x -= 3;
}

// Spawn new pipes
if (pipes.back().x < SCREEN_WIDTH - PIPE_GAP) {
    Pipe newPipe;
    newPipe.x = SCREEN_WIDTH;
    newPipe.height = random(50, SCREEN_HEIGHT - 250);
    newPipe.counted = false;
    pipes.push_back(newPipe);
}

// Remove pipes that have gone off-screen
if (pipes.front().x + PIPE_WIDTH < 0) {
    pipes.erase(pipes.begin());
}

// Check for collision with pipes
for (auto& pipe : pipes) {
    if (bird.x + BIRD_WIDTH > pipe.x && bird.x < pipe.x + PIPE_WIDTH) {
        if (bird.y < pipe.height || bird.y + BIRD_HEIGHT > pipe.height + PIPE_GAP) {
            std::cout << "Game over!" << std::endl;
            SDL_Delay(2000);
            exit(0);
        }
    }

    // Count pipes passed
    if (!pipe.counted && pipe.x + PIPE_WIDTH < bird.x) {
        pipe.counted = true;
        std::cout << "Score: " << pipes.size() - 4 << std::endl;
    }
}
}

void render(Bird& bird, std::vector<Pipe>& pipes) {
// Clear screen
SDL_RenderClear(gRenderer);
// Render background
SDL_RenderCopy(gRenderer, gBackgroundTexture, NULL, NULL);

// Render pipes
for (auto& pipe : pipes) {
    SDL_Rect pipeRect = { pipe.x, 0, PIPE_WIDTH, pipe.height };
    SDL_RenderCopy(gRenderer, gPipeTexture, NULL, &pipeRect);

    SDL_Rect pipeRect2 = { pipe.x, pipe.height + PIPE_GAP, PIPE_WIDTH, SCREEN_HEIGHT - (pipe.height + PIPE_GAP) };
    SDL_RenderCopy(gRenderer, gPipeTexture, NULL, &pipeRect2);
}

// Render bird
SDL_Rect birdRect = { bird.x, bird.y, BIRD_WIDTH, BIRD_HEIGHT };
SDL_RenderCopy(gRenderer, gBirdTexture, NULL, &birdRect);

// Update screen
SDL_RenderPresent(gRenderer);
}

int random(int min, int max) {
return min + rand() % (max - min + 1);
}

int main(int argc, char* argv[]) {
srand(time(nullptr));
if (!init() || !loadMedia()) {
    std::cout << "Failed to initialize or load media" << std::endl;
    return 1;
}

Bird bird = { SCREEN_WIDTH / 3, SCREEN_HEIGHT / 2, -10 };
std::vector<Pipe> pipes = { { SCREEN_WIDTH, SCREEN_HEIGHT / 2, false }, { SCREEN_WIDTH + SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3, false }, {
SCREEN_WIDTH + SCREEN_WIDTH, SCREEN_HEIGHT - SCREEN_HEIGHT / 4, false } };

bool quit = false;
SDL_Event e;

while (!quit) {
    // Handle events
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        } else if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_SPACE) {
                bird.velocity = -10;
            }
        }
    }

    // Update game
    update(bird, pipes);

    // Render game
    render(bird, pipes);

    // Wait for a short time
    SDL_Delay(10);
}

close();

return 0;
}
