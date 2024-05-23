#ifndef FIELDCOMPONENT_HPP
#define FIELDCOMPONENT_HPP

#include "Component.hpp"
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>

class FieldComponent : public Component {
public:
    std::unordered_map<std::string, std::vector<glm::vec3>> fields;
    int width, height, depth;

    FieldComponent(int width, int height, int depth);
    glm::vec3 getField(const std::string& fieldType, const glm::vec3& position) const;
    void addField(const std::string& fieldType, const std::function<glm::vec3(int, int, int)>& generator);
    void updateField(const std::string& fieldType);

private:
    int index(int x, int y, int z) const;
};

#endif // FIELDCOMPONENT_HPP
