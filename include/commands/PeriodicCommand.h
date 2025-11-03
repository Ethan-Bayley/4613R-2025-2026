#pragma once
#include "commands/Command.h"
#include "pros/rtos.hpp"
#include <functional>


class PeriodicCommand : public Command {
public:
  PeriodicCommand(
    std::function<void()> onRun,
    const std::string& name = "PeriodicCommand",
    uint32_t minIntervalMs = 0
  );

private:
  std::function<void()> userFunction;
  uint32_t minIntervalMs;
  uint32_t lastExecutionTime;
};
