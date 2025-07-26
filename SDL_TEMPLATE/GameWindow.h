#pragma once
#include <SDL.h>

class GameWindow {
public:
    // Constructor and Destructor
    GameWindow();
    ~GameWindow();

    // Initializes a window
    bool init();
    bool initOpenGLContext();
    void setupDraw();

    // Handles window events
    void handleEvent(SDL_Event& e);

    // Brings the window to focus
    void focus();

    // Releases resources
    void free();

    // Toggle Fullscreen
    void toggleFullscreen();

    void update();
    void render();

    // Getters
    int width();
    int height();
    bool hasMouseFocus();
    bool hasKeyboardFocus();
    bool isMinimized();
    bool isShown();
    SDL_Window* getWindow();
    SDL_GLContext& getGLContext();

private:
    void initVariables();
    void initOpenGlContextSetup();

private:
    SDL_Window* mWindow;        // Pointer to SDL window
    int mWindowID;              // Unique ID for this window
    int mWidth, mHeight;        // Dimensions

    bool mMouseFocus;           // Mouse focus flag
    bool mKeyboardFocus;        // Keyboard focus flag
    bool mFullScreen;           // Fullscreen state
    bool mMinimized;            // Minimized state
    bool mShown;                // Visibility state

    SDL_GLContext openGLContext;
};