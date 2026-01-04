#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <unordered_map>
#include <vector>
#include <functional>
#include <glm/glm.hpp>

/**
 * @brief Enumeration for keyboard keys
 */
enum class Key {
    // Letter keys
    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    
    // Number keys
    NUM_0, NUM_1, NUM_2, NUM_3, NUM_4,
    NUM_5, NUM_6, NUM_7, NUM_8, NUM_9,
    
    // Function keys
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
    
    // Special keys
    ESCAPE, ENTER, TAB, BACKSPACE, SPACE,
    SHIFT, CTRL, ALT,
    ARROW_UP, ARROW_DOWN, ARROW_LEFT, ARROW_RIGHT,
    HOME, END, PAGE_UP, PAGE_DOWN,
    DELETE, INSERT,
    
    // Unknown key
    UNKNOWN
};

/**
 * @brief Enumeration for mouse buttons
 */
enum class MouseButton {
    LEFT,
    RIGHT,
    MIDDLE,
    UNKNOWN
};

/**
 * @brief Enumeration for input action types
 */
enum class InputAction {
    PRESSED,
    RELEASED,
    HELD
};

/**
 * @brief Callback function type for keyboard events
 */
using KeyCallback = std::function<void(Key, InputAction)>;

/**
 * @brief Callback function type for mouse movement events
 */
using MouseMovementCallback = std::function<void(const glm::vec2&)>;

/**
 * @brief Callback function type for mouse button events
 */
using MouseButtonCallback = std::function<void(MouseButton, InputAction)>;

/**
 * @brief Callback function type for mouse scroll events
 */
using MouseScrollCallback = std::function<void(float)>;

/**
 * @class InputHandler
 * @brief Handles keyboard and mouse input for the Cppcraft-2 application
 * 
 * This class manages all input events from the keyboard and mouse,
 * maintains the state of keys and buttons, and provides mechanisms
 * for registering and triggering callbacks.
 */
class InputHandler {
public:
    /**
     * @brief Constructor
     */
    InputHandler();
    
    /**
     * @brief Destructor
     */
    ~InputHandler();
    
    /**
     * @brief Initialize the input handler
     * @return true if initialization was successful, false otherwise
     */
    bool initialize();
    
    /**
     * @brief Shutdown the input handler
     */
    void shutdown();
    
    /**
     * @brief Update input states (called once per frame)
     */
    void update();
    
    // ==================== Keyboard Methods ====================
    
    /**
     * @brief Register a keyboard callback
     * @param key The key to listen for
     * @param callback The callback function to invoke
     * @return The ID of the registered callback for later removal
     */
    int registerKeyCallback(Key key, const KeyCallback& callback);
    
    /**
     * @brief Unregister a keyboard callback
     * @param key The key to stop listening for
     * @param callbackId The ID of the callback to remove
     */
    void unregisterKeyCallback(Key key, int callbackId);
    
    /**
     * @brief Check if a key is currently pressed
     * @param key The key to check
     * @return true if the key is pressed, false otherwise
     */
    bool isKeyPressed(Key key) const;
    
    /**
     * @brief Check if a key is currently held down
     * @param key The key to check
     * @return true if the key is held, false otherwise
     */
    bool isKeyHeld(Key key) const;
    
    /**
     * @brief Check if a key was released
     * @param key The key to check
     * @return true if the key was released, false otherwise
     */
    bool isKeyReleased(Key key) const;
    
    /**
     * @brief Get all currently pressed keys
     * @return Vector of currently pressed keys
     */
    std::vector<Key> getPressedKeys() const;
    
    // ==================== Mouse Methods ====================
    
    /**
     * @brief Register a mouse movement callback
     * @param callback The callback function to invoke
     * @return The ID of the registered callback for later removal
     */
    int registerMouseMovementCallback(const MouseMovementCallback& callback);
    
    /**
     * @brief Unregister a mouse movement callback
     * @param callbackId The ID of the callback to remove
     */
    void unregisterMouseMovementCallback(int callbackId);
    
    /**
     * @brief Register a mouse button callback
     * @param button The mouse button to listen for
     * @param callback The callback function to invoke
     * @return The ID of the registered callback for later removal
     */
    int registerMouseButtonCallback(MouseButton button, const MouseButtonCallback& callback);
    
    /**
     * @brief Unregister a mouse button callback
     * @param button The mouse button to stop listening for
     * @param callbackId The ID of the callback to remove
     */
    void unregisterMouseButtonCallback(MouseButton button, int callbackId);
    
    /**
     * @brief Register a mouse scroll callback
     * @param callback The callback function to invoke
     * @return The ID of the registered callback for later removal
     */
    int registerMouseScrollCallback(const MouseScrollCallback& callback);
    
    /**
     * @brief Unregister a mouse scroll callback
     * @param callbackId The ID of the callback to remove
     */
    void unregisterMouseScrollCallback(int callbackId);
    
    /**
     * @brief Get the current mouse position
     * @return The mouse position in screen coordinates
     */
    glm::vec2 getMousePosition() const;
    
    /**
     * @brief Get the mouse position change since the last frame
     * @return The delta movement of the mouse
     */
    glm::vec2 getMouseDelta() const;
    
    /**
     * @brief Check if a mouse button is pressed
     * @param button The mouse button to check
     * @return true if the button is pressed, false otherwise
     */
    bool isMouseButtonPressed(MouseButton button) const;
    
    /**
     * @brief Check if a mouse button is held
     * @param button The mouse button to check
     * @return true if the button is held, false otherwise
     */
    bool isMouseButtonHeld(MouseButton button) const;
    
    /**
     * @brief Set mouse position (if supported by platform)
     * @param position The new mouse position
     */
    void setMousePosition(const glm::vec2& position);
    
    /**
     * @brief Show or hide the mouse cursor
     * @param visible true to show, false to hide
     */
    void setMouseVisible(bool visible);
    
    /**
     * @brief Check if the mouse is visible
     * @return true if visible, false otherwise
     */
    bool isMouseVisible() const;
    
    // ==================== General Methods ====================
    
    /**
     * @brief Clear all input states and callbacks
     */
    void clear();
    
    /**
     * @brief Check if any input is being received
     * @return true if any key or mouse button is active
     */
    bool hasActiveInput() const;

private:
    // Keyboard state tracking
    std::unordered_map<int, bool> keyStates_;           ///< Current state of keys
    std::unordered_map<int, bool> previousKeyStates_;   ///< Previous frame key states
    
    // Keyboard callbacks
    std::unordered_map<int, std::vector<std::pair<int, KeyCallback>>> keyCallbacks_;
    
    // Mouse state tracking
    glm::vec2 mousePosition_;                            ///< Current mouse position
    glm::vec2 previousMousePosition_;                    ///< Previous frame mouse position
    std::unordered_map<int, bool> mouseButtonStates_;   ///< Current mouse button states
    std::unordered_map<int, bool> previousMouseButtonStates_;  ///< Previous frame button states
    float mouseScrollDelta_;                             ///< Accumulated scroll delta
    bool mouseVisible_;                                  ///< Whether mouse cursor is visible
    
    // Mouse callbacks
    std::vector<std::pair<int, MouseMovementCallback>> mouseMovementCallbacks_;
    std::unordered_map<int, std::vector<std::pair<int, MouseButtonCallback>>> mouseButtonCallbacks_;
    std::vector<std::pair<int, MouseScrollCallback>> mouseScrollCallbacks_;
    
    // Callback ID generation
    int nextCallbackId_;                                 ///< Counter for unique callback IDs
    
    /**
     * @brief Generate a unique callback ID
     * @return A new unique callback ID
     */
    int generateCallbackId();
    
    /**
     * @brief Convert native key code to Key enumeration
     * @param nativeKeyCode The native platform-specific key code
     * @return The corresponding Key enumeration value
     */
    Key convertNativeKeyCode(int nativeKeyCode) const;
    
    /**
     * @brief Convert native mouse button code to MouseButton enumeration
     * @param nativeButtonCode The native platform-specific button code
     * @return The corresponding MouseButton enumeration value
     */
    MouseButton convertNativeMouseButton(int nativeButtonCode) const;
};

#endif // INPUT_HANDLER_H
