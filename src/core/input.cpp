#include "core/input.h"
#include <algorithm>
#include <graphic/graphic.h>
#include <iostream>
#include <core/engine.h>

namespace emp {
    std::vector<KeyInput*> KeyInput::_instances;

    KeyInput::KeyInput(std::vector<int> keysToMonitor) : _isEnabled(true) {
        for (int key : keysToMonitor) {
            _keys[key] = false;
        }
        // Add this instance to the list of instances
        KeyInput::_instances.push_back(this);
    }

    KeyInput::~KeyInput() {
        // Remove this instance from the list of instances
        _instances.erase(std::remove(_instances.begin(), _instances.end(), this), _instances.end());
    }

    bool KeyInput::getIsKeyDown(int key) {
        bool result = false;
        if (_isEnabled) {
            std::map<int, bool>::iterator it = _keys.find(key);
            if (it != _keys.end()) {
                result = _keys[key];
            }
        }
        return result;
    }

    void KeyInput::setIsKeyDown(int key, bool isDown) {
        std::map<int, bool>::iterator it = _keys.find(key);
        if (it != _keys.end()) {
            _keys[key] = isDown;
        }
    }

    GLFWwindow* window = nullptr;

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_E && action == GLFW_PRESS)
            std::cout << "E";
        if (key == GLFW_KEY_Q && action == GLFW_PRESS)
            std::cout << "Q";
    		
        if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS)
            std::cout << "Alt";
        if (key == GLFW_KEY_I && action == GLFW_PRESS)
            std::cout << "I";
        if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
            std::cout << "Left";
        if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
            std::cout << "Right";
        if (key == GLFW_KEY_UP && action == GLFW_PRESS)
            std::cout << "Up";
        if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
            std::cout << "Down";
        // action == GLFW_RELEASE
    }

    void InputSystem::Init()
    {
        window = engine->GetWindow();
        glfwSetKeyCallback(window, key_callback);
    }

    void InputSystem::Update(float)
    {
        glfwPollEvents();
    }

 

    //void KeyInput::setupKeyInputs(GLFWwindow& window) {
    //   // glfwSetKeyCallback(&window, KeyInput::callback);
    //}

    //void KeyInput::callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    //    // Send key event to all KeyInput instances
    //    for (KeyInput* keyInput : _instances) {
    //        keyInput->setIsKeyDown(key, action != GLFW_RELEASE);
    //    }
    //}
}