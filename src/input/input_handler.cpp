#include "input_handler.h"
#include <iostream>
#include <algorithm>

InputHandler::InputHandler()
    : m_isRunning(true),
      m_mouseX(0),
      m_mouseY(0),
      m_mousePressed(false),
      m_scrollDelta(0)
{
    // Initialize key states
    std::fill(m_keyStates.begin(), m_keyStates.end(), false);
}

InputHandler::~InputHandler()
{
    cleanup();
}

bool InputHandler::initialize()
{
    try {
        // Initialize input handling
        m_isRunning = true;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to initialize InputHandler: " << e.what() << std::endl;
        return false;
    }
}

void InputHandler::cleanup()
{
    m_isRunning = false;
    m_keyStates.fill(false);
}

void InputHandler::processInput()
{
    // Process all pending input events
    // This will be called each frame to handle keyboard, mouse, and other input
}

void InputHandler::handleKeyDown(int keyCode)
{
    if (keyCode >= 0 && keyCode < static_cast<int>(m_keyStates.size())) {
        m_keyStates[keyCode] = true;
    }
}

void InputHandler::handleKeyUp(int keyCode)
{
    if (keyCode >= 0 && keyCode < static_cast<int>(m_keyStates.size())) {
        m_keyStates[keyCode] = false;
    }
}

void InputHandler::handleMouseMove(int x, int y)
{
    m_mouseX = x;
    m_mouseY = y;
}

void InputHandler::handleMouseDown(int button)
{
    if (button == 0) { // Left mouse button
        m_mousePressed = true;
    }
}

void InputHandler::handleMouseUp(int button)
{
    if (button == 0) { // Left mouse button
        m_mousePressed = false;
    }
}

void InputHandler::handleMouseScroll(int delta)
{
    m_scrollDelta = delta;
}

bool InputHandler::isKeyPressed(int keyCode) const
{
    if (keyCode >= 0 && keyCode < static_cast<int>(m_keyStates.size())) {
        return m_keyStates[keyCode];
    }
    return false;
}

bool InputHandler::isMousePressed() const
{
    return m_mousePressed;
}

void InputHandler::getMousePosition(int& x, int& y) const
{
    x = m_mouseX;
    y = m_mouseY;
}

int InputHandler::getMouseX() const
{
    return m_mouseX;
}

int InputHandler::getMouseY() const
{
    return m_mouseY;
}

int InputHandler::getScrollDelta() const
{
    return m_scrollDelta;
}

void InputHandler::clearScrollDelta()
{
    m_scrollDelta = 0;
}

void InputHandler::resetInputState()
{
    std::fill(m_keyStates.begin(), m_keyStates.end(), false);
    m_mousePressed = false;
    m_scrollDelta = 0;
}

bool InputHandler::isRunning() const
{
    return m_isRunning;
}

void InputHandler::setRunning(bool running)
{
    m_isRunning = running;
}
