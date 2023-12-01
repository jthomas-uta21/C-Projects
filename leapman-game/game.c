//Compile with : gcc your_source_code.c -o your_program -lSDL2
//make sure SDL is installed on machine

#include <SDL2/SDL.h>

// Define screen dimensions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Define player dimensions
#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 32

// Define enemy dimensions
#define ENEMY_WIDTH 32
#define ENEMY_HEIGHT 32

// Define gravity and jump velocity
#define GRAVITY 0.5
#define JUMP_VELOCITY -10

// Player structure
typedef struct {
    int x, y; // Player position
    int xVel, yVel; // Player velocity
    int isJumping; // Flag to indicate whether the player is jumping
} Player;

// Enemy structure
typedef struct {
    int x, y; // Enemy position
    int xVel; // Enemy velocity
    const char *name; // Enemy name
} Enemy;

// Initialize player
void initPlayer(Player *player) {
    player->x = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2;
    player->y = SCREEN_HEIGHT - PLAYER_HEIGHT;
    player->xVel = 0;
    player->yVel = 0;
    player->isJumping = 0;
}

// Initialize enemy
void initEnemy(Enemy *enemy, int x, int y, const char *name) {
    enemy->x = x;
    enemy->y = y;
    enemy->xVel = 2; // Enemy's initial velocity
    enemy->name = name;
}

// Handle events (keyboard input)
void handleEvents(Player *player, SDL_Event *event) {
    while (SDL_PollEvent(event)) {
        switch (event->type) {
            case SDL_QUIT:
                exit(0);
                break;
            case SDL_KEYDOWN:
                switch (event->key.keysym.sym) {
                    case SDLK_LEFT:
                        player->xVel = -5;
                        break;
                    case SDLK_RIGHT:
                        player->xVel = 5;
                        break;
                    case SDLK_SPACE:
                        if (!player->isJumping) {
                            player->yVel = JUMP_VELOCITY;
                            player->isJumping = 1;
                        }
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (event->key.keysym.sym) {
                    case SDLK_LEFT:
                    case SDLK_RIGHT:
                        player->xVel = 0;
                        break;
                }
                break;
        }
    }
}

// Update player position
void updatePlayer(Player *player) {
    // Update position based on velocity
    player->x += player->xVel;
    player->y += player->yVel;

    // Apply gravity if not jumping
    if (!player->isJumping) {
        player->yVel += GRAVITY;
    }

    // Basic screen boundary check
    if (player->x < 0) {
        player->x = 0;
    }
    if (player->x > SCREEN_WIDTH - PLAYER_WIDTH) {
        player->x = SCREEN_WIDTH - PLAYER_WIDTH;
    }

    // Check if the player has landed
    if (player->y >= SCREEN_HEIGHT - PLAYER_HEIGHT) {
        player->y = SCREEN_HEIGHT - PLAYER_HEIGHT;
        player->yVel = 0;
        player->isJumping = 0;
    }
}

// Update enemy position
void updateEnemy(Enemy *enemy) {
    // Update position based on velocity
    enemy->x += enemy->xVel;

    // Reverse direction if the enemy reaches screen boundaries
    if (enemy->x < 0 || enemy->x > SCREEN_WIDTH - ENEMY_WIDTH) {
        enemy->xVel = -enemy->xVel;
    }
}

// Check for collisions between player and enemy
int checkCollision(Player *player, Enemy *enemy) {
    if (player->x < enemy->x + ENEMY_WIDTH &&
        player->x + PLAYER_WIDTH > enemy->x &&
        player->y < enemy->y + ENEMY_HEIGHT &&
        player->y + PLAYER_HEIGHT > enemy->y) {
        return 1; // Collision detected
    }
    return 0; // No collision
}

// Render player
void renderPlayer(SDL_Renderer *renderer, Player *player) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Set color to red
    SDL_Rect playerRect = {player->x, player->y, PLAYER_WIDTH, PLAYER_HEIGHT};
    SDL_RenderFillRect(renderer, &playerRect); // Render player rectangle
}

// Render enemy
void renderEnemy(SDL_Renderer *renderer, Enemy *enemy) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Set color to blue
    SDL_Rect enemyRect = {enemy->x, enemy->y, ENEMY_WIDTH, ENEMY_HEIGHT};
    SDL_RenderFillRect(renderer, &enemyRect); // Render enemy rectangle
}

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    // Create window and renderer
    SDL_Window *window = SDL_CreateWindow("Mario Clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Initialize player
    Player player;
    initPlayer(&player);

    // Initialize enemies
    Enemy soomba, doopa, clomp;
    initEnemy(&soomba, 50, SCREEN_HEIGHT - ENEMY_HEIGHT, "Soomba");
    initEnemy(&doopa, 200, SCREEN_HEIGHT - ENEMY_HEIGHT, "Doopa");
    initEnemy(&clomp, 400, SCREEN_HEIGHT - ENEMY_HEIGHT, "Clomp");

    // Main game loop
    SDL_Event event;
    while (1) {
        handleEvents(&player, &event); // Handle user input
        updatePlayer(&player); // Update player position
        updateEnemy(&soomba);   // Update enemy position
        updateEnemy(&doopa);
        updateEnemy(&clomp);

        // Check for collisions
        if (checkCollision(&player, &soomba) || checkCollision(&player, &doopa) || checkCollision(&player, &clomp)) {
            printf("Game Over! You collided with an enemy.\n");
            break;
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Render player and enemies
        renderPlayer(renderer, &player);
        renderEnemy(renderer, &soomba);
        renderEnemy(renderer, &doopa);
        renderEnemy(renderer, &clomp);

        // Present the renderer
        SDL_RenderPresent(renderer);

        // Add a small delay to control the frame rate
        SDL_Delay(10);
    }

    // Clean up and quit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
