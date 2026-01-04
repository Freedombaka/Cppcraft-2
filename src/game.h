#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>

/**
 * @class Game
 * @brief Main game class for Cppcraft-2
 * 
 * Handles core game logic, state management, and game loop.
 */
class Game {
public:
    /**
     * @brief Constructor for Game class
     */
    Game();

    /**
     * @brief Destructor for Game class
     */
    ~Game();

    /**
     * @brief Initialize the game
     * @return true if initialization successful, false otherwise
     */
    bool init();

    /**
     * @brief Start the main game loop
     */
    void run();

    /**
     * @brief Shut down the game and cleanup resources
     */
    void shutdown();

    /**
     * @brief Get the current game state
     * @return Current state as a string
     */
    std::string getState() const;

    /**
     * @brief Check if game is running
     * @return true if game is running, false otherwise
     */
    bool isRunning() const;

    /**
     * @brief Update game logic
     * @param deltaTime Time since last update in seconds
     */
    void update(float deltaTime);

    /**
     * @brief Render the game
     */
    void render();

private:
    bool m_running;
    std::string m_state;

    /**
     * @brief Handle input events
     */
    void handleInput();

    /**
     * @brief Process game logic
     * @param deltaTime Time since last update in seconds
     */
    void processLogic(float deltaTime);

    /**
     * @brief Cleanup internal resources
     */
    void cleanup();
};

#endif // GAME_H
