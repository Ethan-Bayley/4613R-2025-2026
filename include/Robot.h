#include "commands/EventLoop.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "subsystems/Subsystems.h"
#include <vector>
#include <memory>
#include <utility>

class Robot
{
public:
    Robot(pros::Controller &controller);
    ~Robot() = default;

    void setup();
    void registerCommands();
    void update();

    void addSubsystem(std::unique_ptr<Subsystems> subsystem)
    {
        robotSubsystems.push_back(std::move(subsystem));
    }

    template <typename T, typename... Args>
    T &addSubsystem(Args &&...args)
    {
        auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
        T &ref = *ptr;
        robotSubsystems.push_back(std::move(ptr));
        return ref;
    }

    template <typename T>
    T *getSubsystem(const std::string &name)
    {
        for (auto &subsystem : robotSubsystems)
        {
            if (auto *ptr = dynamic_cast<T *>(subsystem.get()))
            {
                if (ptr->getName() == name)
                    return ptr;
            }
        }
        return nullptr;
    }

    template <typename AutoType, typename... Args>
    void runAuto(Args &&...args)
    {
        AutoType autonomous(std::forward<Args>(args)...);
        autonomous.run();
    }

private:
    pros::Controller &controller;
    EventLoop eventLoop;
    std::vector<std::unique_ptr<Subsystems>> robotSubsystems;
};