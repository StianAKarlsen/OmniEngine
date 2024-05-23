#ifndef SERVICELOCATOR_HPP
#define SERVICELOCATOR_HPP

#include <memory>
#include <unordered_map>
#include <typeindex>
#include <cassert>
#include <mutex>

class ServiceLocator {
public:
    template<typename T>
    static void provide(std::unique_ptr<T> service) {
        std::lock_guard<std::mutex> lock(mutex_);
        services()[typeid(T)] = std::move(service);
    }

    template<typename T>
    static T& get() {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = services().find(typeid(T));
        assert(it != services().end() && "Service not found!");
        return *static_cast<T*>(it->second.get());
    }

private:
    using ServiceMap = std::unordered_map<std::type_index, std::unique_ptr<void>>;
    static ServiceMap& services() {
        static ServiceMap services;
        return services;
    }

    static std::mutex mutex_;
};

std::mutex ServiceLocator::mutex_;

#endif // SERVICELOCATOR_HPP
