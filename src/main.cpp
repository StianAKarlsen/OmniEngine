#include "Core/ServiceLocator.hpp"
#include "Core/Logger.hpp"
#include "Core/Initialization.hpp"
#include "Core/MainLoop.hpp"
#include "ECS/EntityManager.hpp"
#include "ECS/SystemManager.hpp"
#include "Rendering/Renderer.hpp"
#include "Physics/PhysicsEngine.hpp"
#include "Audio/AudioEngine.hpp"
#include "Input/InputMapper.hpp"
#include "AI/AISystem.hpp"
#include "UI/UISystem.hpp"
#include "Systems/ResourceManager.hpp"
#include "Systems/EventManager.hpp"
#include "Systems/NetworkSystem.hpp"
#include "Systems/ScriptingSystem.hpp"
#include "Systems/DebugSystem.hpp"
#include "Components/PositionComponent.hpp"
#include "Components/VelocityComponent.hpp"
#include "Components/MassComponent.hpp"
#include "Components/FieldComponent.hpp"
#include "Systems/UniverseSystem.hpp"
#include "Systems/RenderSystem.hpp"
#include "Systems/ParticleSystem.hpp"
#include <random>

void setupDemoScene() {
    auto& entityManager = Core::ServiceLocator::get<ECS::EntityManager>();
    auto& systemManager = Core::ServiceLocator::get<ECS::SystemManager>();

    auto& universeSystem = systemManager.getSystem<Systems::UniverseSystem>();
    auto& renderSystem = systemManager.getSystem<Systems::RenderSystem>();
    auto& particleSystem = systemManager.getSystem<Systems::ParticleSystem>();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 4);

    auto fieldComponent = std::make_shared<Components::FieldComponent>(100, 100, 100);
    fieldComponent->addField("gravitational", [](int x, int y, int z) {
        return glm::vec3(0.0f, -9.81f, 0.0f);
    });
    fieldComponent->addField("electromagnetic", [](int x, int y, int z) {
        return glm::vec3(0.0f, 0.0f, 0.1f);
    });

    entityManager.createEntity().addComponent(fieldComponent);
    particleSystem.setFieldComponent(fieldComponent);

    for (int i = 0; i < 10; ++i) {
        auto& entity = entityManager.createEntity();
        auto& position = entity.addComponent<Components::PositionComponent>();
        auto& velocity = entity.addComponent<Components::VelocityComponent>();
        auto& mass = entity.addComponent<Components::MassComponent>();
        auto& customFieldComponent = entity.addComponent<Components::FieldComponent>();

        position.position = {static_cast<float>(i * 2), 0.0f, 0.0f};
        velocity.velocity = {0.0f, 0.0f, 0.0f};
        mass.mass = static_cast<float>(dist(gen));

        // Add random fields
        customFieldComponent.fields["gravitational"] = fieldComponent->fields["gravitational"];
        if (dist(gen) % 2 == 1) {
            customFieldComponent.fields["electromagnetic"] = fieldComponent->fields["electromagnetic"];
        }

        universeSystem.addEntity(&entity);
        renderSystem.addComponent(entity.getComponent<Components::PositionComponent>());
        particleSystem.addField(entity.getComponent<Components::FieldComponent>());
    }

    // Add particles managed entirely on the GPU
    particleSystem.addParticles(1000000); // Add 1 million particles
}

void setupInputMapping() {
    auto& inputMapper = Core::ServiceLocator::get<InputMapper>();
    inputMapper.mapAction("MoveForward", GLFW_KEY_W);
    inputMapper.mapAction("MoveBackward", GLFW_KEY_S);
    inputMapper.mapAction("MoveLeft", GLFW_KEY_A);
    inputMapper.mapAction("MoveRight", GLFW_KEY_D);
}

int main() {
    Core::Initialization::initialize();

    Core::ServiceLocator::provide(std::make_unique<ECS::EntityManager>());
    Core::ServiceLocator::provide(std::make_unique<ECS::SystemManager>());
    Core::ServiceLocator::provide(std::make_unique<Rendering::Renderer>());
    Core::ServiceLocator::provide(std::make_unique<Systems::UniverseSystem>());
    Core::ServiceLocator::provide(std::make_unique<Systems::ParticleSystem>());
    Core::ServiceLocator::provide(std::make_unique<Systems::PhysicsEngine>());
    Core::ServiceLocator::provide(std::make_unique<Systems::AudioEngine>());
    Core::ServiceLocator::provide(std::make_unique<InputMapper>());
    Core::ServiceLocator::provide(std::make_unique<AISystem>());
    Core::ServiceLocator::provide(std::make_unique<UISystem>());
    Core::ServiceLocator::provide(std::make_unique<ResourceManager>());
    Core::ServiceLocator::provide(std::make_unique<EventManager>());
    Core::ServiceLocator::provide(std::make_unique<NetworkSystem>());
    Core::ServiceLocator::provide(std::make_unique<ScriptingSystem>());
    Core::ServiceLocator::provide(std::make_unique<DebugSystem>());

    auto& systemManager = Core::ServiceLocator::get<ECS::SystemManager>();
    systemManager.addSystem<Systems::UniverseSystem>();
    systemManager.addSystem<Systems::RenderSystem>();
    systemManager.addSystem<Systems::ParticleSystem>();
    systemManager.addSystem<Systems::PhysicsEngine>();
    systemManager.addSystem<Systems::AudioEngine>();
    systemManager.addSystem<Systems::AISystem>();
    systemManager.addSystem<Systems::UISystem>();
    systemManager.addSystem<Systems::ResourceManager>();
    systemManager.addSystem<Systems::EventManager>();
    systemManager.addSystem<Systems::NetworkSystem>();
    systemManager.addSystem<Systems::ScriptingSystem>();
    systemManager.addSystem<Systems::DebugSystem>();

    Core::Logger::log(Core::INFO, "Starting Engine");

    setupDemoScene();
    setupInputMapping();

    Core::MainLoop mainLoop;
    mainLoop.run();

    Core::Logger::log(Core::INFO, "Engine Shutdown");

    Core::Initialization::cleanup();

    return 0;
}
