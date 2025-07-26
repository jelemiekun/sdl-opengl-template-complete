#include "Game.h"
#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include "GameWindow.h"
#include "ImGuiWindow.h"
#include "imgui/imgui_impl_sdl2.h"

Game::Game() : running(false), gameWindow(nullptr), imGuiWindow(nullptr) {}

Game* Game::getInstance() {
    static Game instance;
    return &instance;
}

void Game::setOpenGLAttributes() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

bool Game::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        spdlog::error("SDL Failed to initialize: {}", SDL_GetError());
        return false;
    } else {
        spdlog::info("SDL initialized successfully.");
    }
    setOpenGLAttributes();
    
    gameWindow = new GameWindow;

    if (gameWindow->init()) {
        spdlog::info("gameWindow initialized successfully.");
    } else {
        spdlog::error("gameWindow failed to initialize.");
        return false;
    }

    return true;
}

bool Game::initGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        spdlog::error("Failed to initialize GLAD.");
        return false;
    }
    spdlog::info("GLAD initialized successfully.");
    return true;
}

void Game::initImGui() {
    imGuiWindow = ImGuiWindow::getInstance();
    imGuiWindow->init(gameWindow->getWindow(), gameWindow->getGLContext());
}

void Game::initializeEverything() {
    spdlog::info("Initializing program...");
    bool initializationSuccess = initSDL() && gameWindow->initOpenGLContext() && initGLAD();

    if (initializationSuccess) {
        initImGui();

        gameWindow->setupDraw();

        spdlog::info("Program initialized successfully.");
        running = true;
    } else {
        spdlog::error("Program failed to initialize.");
        running = false;
    }
}

void Game::input() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;
        ImGui_ImplSDL2_ProcessEvent(&event);
        gameWindow->handleEvent(event);
    }
}

void Game::update() {
    gameWindow->update();
}

void Game::render() {
    gameWindow->render();
}

const bool& Game::isRunning() const {
    return running;
}

void Game::setRunning(bool value) {
    running = value;
}

void Game::clean() {
    gameWindow->free();
    imGuiWindow->clean();
}
