#ifndef SYSTEM_HPP
#define SYSTEM_HPP

class System {
public:
    virtual ~System() = default;
    virtual void update() = 0;
    virtual void render() = 0;
};

#endif // SYSTEM_HPP
