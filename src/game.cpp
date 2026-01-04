#include "game.h"
#include <iostream>
#include <stdexcept>

Game::Game() : isRunning(false), isInitialized(false), windowWidth(800), windowHeight(600) {
    std::cout << "Game object created" << std::endl;
}

Game::~Game() {
    if (isInitialized) {
        shutdown();
    }
    std::cout << "Game object destroyed" << std::endl;
}

bool Game::initialize() {
    if (isInitialized) {
        std::cerr << "Game is already initialized" << std::endl;
        return false;
    }

    try {
        // Initialize game systems
        std::cout << "Initializing game systems..." << std::endl;
        
        // Initialize window/graphics
        if (!initializeGraphics()) {
            std::cerr << "Failed to initialize graphics" << std::endl;
            return false;
        }

        // Initialize input system
        if (!initializeInput()) {
            std::cerr << "Failed to initialize input" << std::endl;
            cleanup();
            return false;
        }

        // Initialize game logic
        if (!initializeGameLogic()) {
            std::cerr << "Failed to initialize game logic" << std::endl;
            cleanup();
            return false;
        }

        isInitialized = true;
        std::cout << "Game initialized successfully" << std::endl;
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Exception during game initialization: " << e.what() << std::endl;
        cleanup();
        return false;
    }
}

bool Game::initializeGraphics() {
    std::cout << "Initializing graphics system..." << std::endl;
    // Graphics initialization code here
    return true;
}

bool Game::initializeInput() {
    std::cout << "Initializing input system..." << std::endl;
    // Input system initialization code here
    return true;
}

bool Game::initializeGameLogic() {
    std::cout << "Initializing game logic..." << std::endl;
    // Game logic initialization code here
    return true;
}

void Game::cleanup() {
    std::cout << "Cleaning up game resources..." << std::endl;
    // Add cleanup code here
}

void Game::shutdown() {
    if (!isInitialized) {
        return;
    }

    std::cout << "Shutting down game..." << std::endl;
    isRunning = false;
    cleanup();
    isInitialized = false;
    std::cout << "Game shutdown complete" << std::endl;
}

bool Game::run() {
    if (!isInitialized) {
        std::cerr << "Game must be initialized before running" << std::endl;
        return false;
    }

    isRunning = true;
    std::cout << "Game loop started" << std::endl;

    while (isRunning) {
        if (!update()) {
            break;
        }
        if (!render()) {
            break;
        }
    }

    std::cout << "Game loop ended" << std::endl;
    return true;
}

bool Game::update() {
    // Handle input
    handleInput();

    // Update game logic
    updateGameLogic();

    return isRunning;
}

bool Game::render() {
    // Clear screen
    clearScreen();

    // Render game objects
    renderGame();

    // Present frame
    presentFrame();

    return true;
}

void Game::handleInput() {
    // Input handling code here
}

void Game::updateGameLogic() {
    // Game logic update code here
}

void Game::clearScreen() {
    // Clear screen/backbuffer code here
}

void Game::renderGame() {
    // Render all game objects here
}

void Game::presentFrame() {
    // Present the rendered frame to the display
}

void Game::exit() {
    isRunning = false;
}

bool Game::isRunning_() const {
    return isRunning;
}

bool Game::isInitialized_() const {
    return isInitialized;
}

int Game::getWindowWidth() const {
    return windowWidth;
}

int Game::getWindowHeight() const {
    return windowHeight;
}

void Game::setWindowSize(int width, int height) {
    if (width > 0 && height > 0) {
        windowWidth = width;
        windowHeight = height;
        std::cout << "Window size set to " << width << "x" << height << std::endl;
    } else {
        std::cerr << "Invalid window dimensions" << std::endl;
    }
}
