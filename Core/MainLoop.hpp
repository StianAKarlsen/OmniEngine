#ifndef CORE_MAINLOOP_HPP
#define CORE_MAINLOOP_HPP

#include "ServiceLocator.hpp"
#include "Logger.hpp"
#include "ECS/EntityManager.hpp"
#include "ECS/SystemManager.hpp"

namespace Core {

class MainLoop {
public:
    void run() {
        while (true) { // Simple infinite loop for demo purposes
            auto& entityManager = ServiceLocator::get<ECS::EntityManager>();
            auto& systemManager = ServiceLocator::get<ECS::SystemManager>();
            
            // Update systems
            systemManager.update();

            // Render systems
            systemManager.render();
        }
    }
};

} // namespace Core

#endif // CORE_MAINLOOP_HPP
