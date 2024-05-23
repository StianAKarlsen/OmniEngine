#ifndef INPUTMAPPER_HPP
#define INPUTMAPPER_HPP

#include <unordered_map>
#include <functional>
#include <GLFW/glfw3.h>

namespace Systems {

class InputMapper {
public:
    using Action = std::function<void()>;

    void mapAction(const std::string& actionName, int key);
    void setAction(const std::string& actionName, Action action);
    void handleInput();

private:
    std::unordered_map<std::string, int> actionMappings;
    std::unordered_map<std::string, Action> actions;
};

} // namespace Systems

#endif // INPUTMAPPER_HPP
