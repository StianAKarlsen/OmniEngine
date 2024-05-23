#include "InputMapper.hpp"

namespace Systems {

void InputMapper::mapAction(const std::string& actionName, int key) {
    actionMappings[actionName] = key;
}

void InputMapper::setAction(const std::string& actionName, Action action) {
    actions[actionName] = std::move(action);
}

void InputMapper::handleInput() {
    for (const auto& [actionName, key] : actionMappings) {
        if (glfwGetKey(glfwGetCurrentContext(), key) == GLFW_PRESS) {
            if (actions.find(actionName) != actions.end()) {
                actions[actionName]();
            }
        }
    }
}

} // namespace Systems
