#ifndef PARTICLESYSTEM_HPP
#define PARTICLESYSTEM_HPP

#include "System.hpp"
#include "Components/FieldComponent.hpp"
#include <vector>
#include <memory>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <atomic>

class ParticleSystem : public System {
public:
    ParticleSystem();
    ~ParticleSystem();

    void setFieldComponent(std::shared_ptr<FieldComponent> fieldComponent);
    void addParticles(int count);
    void update() override;
    void render() override;

private:
    GLuint VAO, VBO, SSBO;
    GLuint fieldSSBO;
    GLuint computeShader, renderShader;
    std::atomic<int> numParticles;
    std::shared_ptr<FieldComponent> fieldComponent;

    void initOpenGL();
    void initShaders();
    void updateParticles();
    void handleCollisions();
    void updateFieldData();
};

#endif // PARTICLESYSTEM_HPP
