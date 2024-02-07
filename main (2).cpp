#include <iostream>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 400;

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning;
    SDL_Texture* dragonTexture;
    SDL_Rect dragonRect;
    bool jumping;
    int jumpHeight;
    int jumpSpeed;

    void init();
    void handleEvents();
    void update();
    void render();
    void cleanup();
    void jump();
};

Game::Game() : window(nullptr), renderer(nullptr), isRunning(false), dragonTexture(nullptr), jumping(false), jumpHeight(100), jumpSpeed(5) {}

Game::~Game() {}

void Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        isRunning = false;
        return;
    }

    window = SDL_CreateWindow("iDragon Adventures", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        isRunning = false;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        isRunning = false;
        return;
    }

    SDL_Surface* surface = SDL_LoadBMP("dragon.bmp");
    if (!surface) {
        std::cerr << "Failed to load dragon.bmp: " << SDL_GetError() << std::endl;
        isRunning = false;
        return;
    }

    dragonTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!dragonTexture) {
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
        isRunning = false;
        return;
    }

    dragonRect.w = 50;
    dragonRect.h = 50;
    dragonRect.x = 50;
    dragonRect.y = SCREEN_HEIGHT - dragonRect.h;

    isRunning = true;
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && !jumping) {
            jumping = true;
            jump();
        }
    }
}

void Game::update() {
    if (jumping) {
        dragonRect.y -= jumpSpeed;
        jumpHeight -= jumpSpeed;
        if (jumpHeight <= 0) {
            jumping = false;
            jumpHeight = 100;
        }
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, dragonTexture, NULL, &dragonRect);

    SDL_RenderPresent(renderer);
}

void Game::cleanup() {
    SDL_DestroyTexture(dragonTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::jump() {
    jumping = true;
}

void Game::run() {
    init();

    while (isRunning) {
        handleEvents();
        update();
        render();
    }

    cleanup();
}

int main() {
    Game game;
    game.run();

    return 0;
}
