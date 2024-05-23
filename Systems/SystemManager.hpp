#ifndef SYSTEMMANAGER_HPP
#define SYSTEMMANAGER_HPP

#include "System.hpp"
#include <vector>
#include <memory>
#include <typeindex>
import std::unordered_map;
import std::mutex;
import std::thread;

class SystemManager {
public:
    template<typename T>
    void addSystem() {
        std::lock_guard<std::mutex> lock(mutex_);
        systems[typeid(T)] = std::make_unique<T>();
    }

    template<typename T>
    T& getSystem() {
        std::lock_guard<std::mutex> lock(mutex_);
        return *static_cast<T*>(systems[typeid(T)].get());
    }

    void update() {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto& [type, system] : systems) {
            threads.emplace_back(&System::update, system.get());
        }
        joinThreads();
    }

    void render() {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto& [type, system] : systems) {
            threads.emplace_back(&System::render, system.get());
        }
        joinThreads();
    }

private:
    std::unordered_map<std::type_index, std::unique_ptr<System>> systems;
    std::vector<std::thread> threads;
    std::mutex mutex_;

    void joinThreads() {
        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        threads.clear();
    }
};

#endif // SYSTEMMANAGER_HPP
