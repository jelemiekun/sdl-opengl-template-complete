#include "GameWindow.h"
#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <memory>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Shader.h"
#include "Texture2D.h"
#include "Camera.h"
#include "Game.h"
#include "ImGuiWindow.h"
#include "ProgramValues.h"
#include "Model.h"

static Game* game = Game::getInstance();

static constexpr int INITIAL_WIDTH = 1280;
static constexpr int INITIAL_HEIGHT = 720;

static std::unique_ptr<Model> model1;
static std::unique_ptr<Model> model2;

static std::unique_ptr<Camera> camera;

static std::unique_ptr<Shader> shaderObject;

static std::unique_ptr<Shader> shaderLight;

// Constructor
GameWindow::GameWindow()
    : mWindow(nullptr), openGLContext(nullptr), mWindowID(-1),
    mWidth(0), mHeight(0), mMouseFocus(false), mKeyboardFocus(false),
    mFullScreen(false), mMinimized(false), mShown(false) {}

// Destructor
GameWindow::~GameWindow() {
    free();
}

// Initializes the window
bool GameWindow::init() {
    mWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        INITIAL_WIDTH, INITIAL_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (!mWindow) {
        spdlog::error("Window creation failed: {}", SDL_GetError());
        return false;
    }

    initVariables();

    spdlog::info("Window initialized successfully");
    return true;
}

void GameWindow::setupDraw() {
    camera = std::make_unique<Camera>(
        glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        -90.0f,
        0.0f
    );

    shaderObject = std::make_unique<Shader>("source.shader");

    shaderLight = std::make_unique<Shader>("light.shader");

    std::vector<GLfloat> verticesLight = {
        // Position                Normal
            -0.2f,  0.2f,  0.2f,
             0.2f,  0.2f,  0.2f,
            -0.2f, -0.2f,  0.2f,
             0.2f, -0.2f,  0.2f,

             -0.2f,  0.2f, -0.2f,
              0.2f,  0.2f, -0.2f,
             -0.2f, -0.2f, -0.2f,
              0.2f, -0.2f, -0.2f
    };

    model1 = std::make_unique<Model>("assets/models/military_backpack/scene.gltf");
    model2 = std::make_unique<Model>("assets/models/Donut.glb");
}

// Brings the window into focus
void GameWindow::focus() {
    if (!mShown) {
        SDL_ShowWindow(mWindow);
        spdlog::info("Window {} shown from focus()", mWindowID);
    }
    SDL_RaiseWindow(mWindow);
}

// Frees resources
void GameWindow::free() {
    if (mWindow) {
        spdlog::info("Destroying window {}", mWindowID);
        SDL_GL_DeleteContext(openGLContext);
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
    }
    mWidth = mHeight = 0;
}

void GameWindow::toggleFullscreen() {
    if (mFullScreen) {
        SDL_SetWindowFullscreen(mWindow, 0);  // Remove fullscreen
        mFullScreen = false;
        spdlog::info("Fullscreen disabled for window {}", mWindowID);
    } else {
        SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP); // Set fullscreen
        mFullScreen = true;
        spdlog::info("Fullscreen enabled for window {}", mWindowID);
    }
}

// Handles events specific to this window
void GameWindow::handleEvent(SDL_Event& e) {
    if (e.type == SDL_WINDOWEVENT && e.window.windowID == mWindowID) {
        switch (e.window.event) {
            case SDL_WINDOWEVENT_SHOWN:
                mShown = true;
                spdlog::info("Window {} shown", mWindowID);
                break;

            case SDL_WINDOWEVENT_HIDDEN:
                mShown = false;
                spdlog::info("Window {} hidden", mWindowID);
                break;

            case SDL_WINDOWEVENT_SIZE_CHANGED:
                mWidth = e.window.data1;
                mHeight = e.window.data2;
                glViewport(0, 0, mWidth, mHeight);
                spdlog::info("Window {} resized to {}x{}", mWindowID, mWidth, mHeight);
                break;

            case SDL_WINDOWEVENT_RESTORED:
                ProgramValues::KeyEvents::isLockedIn = true;
                spdlog::info("Window {} restored", mWindowID);
                break;

            case SDL_WINDOWEVENT_CLOSE:
                game->setRunning(false);
                SDL_HideWindow(mWindow);
                mShown = false;
                spdlog::info("Window {} closed and program stopping", mWindowID);
                break;
        }
    }

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F11) {
        toggleFullscreen();
        spdlog::info("Toggled fullscreen mode");
    }

    camera->processKeyboard(e, this);
    camera->processMouseMotion(e);
}

void GameWindow::update() {
    camera->update();

    shaderObject->use();
    shaderObject->setVec3("u_CameraPos", camera->position);
    shaderObject->setFloat("material.shininess", ProgramValues::Object::shininess);

    { // Directional Light
        shaderObject->setVec3("dirLight.direction", -glm::vec3(0.0f, 1.0f, 0.0f));
        shaderObject->setVec3("dirLight.ambient", glm::vec3(0.08f));
        shaderObject->setVec3("dirLight.diffuse", glm::vec3(0.3f));
        shaderObject->setVec3("dirLight.specular", glm::vec3(0.5f));
    }
}

void GameWindow::render() {
    // TODO: bawat object/model may sariling mat4 transformation
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    

    glm::mat4 model = glm::mat4(1.0f);
    //model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
    shaderObject->setFloat("material.shininess", 32.0f);
    model1->Draw(*shaderObject, model);
    
    
    model = glm::mat4(1.0f);
    //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 10.0f));
    model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
    shaderObject->setFloat("material.shininess", 32.0f);
    model2->Draw(*shaderObject, model);
    
    glm::mat4 projection = glm::perspective(
        glm::radians(camera->getFOV()), 
        (float)mWidth / (float)mHeight,
        0.1f, 
        2000.0f
    );
    glm::mat4 objectModel = glm::mat4(1.0f);

    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
    
    shaderObject->setMat4("u_Projection", projection);
    shaderObject->setMat4("u_View", camera->getViewMatrix());
    shaderObject->setMat3("u_NormalMatrix", normalMatrix);

    game->imGuiWindow->render();
    SDL_GL_SwapWindow(mWindow);
}

// Getters
int GameWindow::width() { return mWidth; }
int GameWindow::height() { return mHeight; }
bool GameWindow::hasMouseFocus() { return mMouseFocus; }
bool GameWindow::hasKeyboardFocus() { return mKeyboardFocus; }
bool GameWindow::isMinimized() { return mMinimized; }
bool GameWindow::isShown() { return mShown; }

SDL_Window* GameWindow::getWindow() {
    return mWindow ? mWindow : nullptr;
}

SDL_GLContext& GameWindow::getGLContext() {
    return openGLContext;
}

void GameWindow::initVariables() {
    mMouseFocus = mKeyboardFocus = true;
    mShown = true;
    mWidth = INITIAL_WIDTH;
    mHeight = INITIAL_HEIGHT;
    mWindowID = SDL_GetWindowID(mWindow);
}

void GameWindow::initOpenGlContextSetup() {
    glViewport(0, 0, 1280, 720);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

bool GameWindow::initOpenGLContext() {
    openGLContext = SDL_GL_CreateContext(mWindow);
    if (!openGLContext) {
        spdlog::error("SDL_GL_CreateContext failed: {}", SDL_GetError());
        return false;
    }

    if (SDL_GL_MakeCurrent(mWindow, openGLContext) < 0) {
        spdlog::error("SDL_GL_MakeCurrent failed: {}", SDL_GetError());
        return false;
    }

    spdlog::info("SDL_GL_CreateContext on Window {} initialized successfully.", mWindowID);
    return true;
}
