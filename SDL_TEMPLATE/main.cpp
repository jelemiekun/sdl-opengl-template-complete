#include "Game.h"
#include "FPSManager.h"

int SDL_main(int argc, char* argv[]) {
    Game* game = Game::getInstance();

    game->initializeEverything();

    int countFrame = 0;
    Uint32 startTime = SDL_GetTicks();

    while (game->isRunning()) {
        Uint32 frameStart = SDL_GetTicks();

        game->input();
        game->update();
        game->render();

        FPSManager::limitFPS(frameStart);
        FPSManager::calculateAverageFPS(countFrame, startTime);
    }

    game->clean();

    return 0;
}