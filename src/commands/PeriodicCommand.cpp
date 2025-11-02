#include "commands/PeriodicCommand.h"
#include "pros/rtos.hpp"

PeriodicCommand::PeriodicCommand(
  std::function<void()> onRun,
  const std::string& name,
  uint32_t minIntervalMs
) : Command(name, nullptr),
    userFunction(onRun),
    minIntervalMs(minIntervalMs),
    lastExecutionTime(0)
{
  executeFunc = [this]() {
    uint32_t currentTime = pros::millis();

    // Check time since last excecution
    if (this->minIntervalMs == 0 || (currentTime - this->lastExecutionTime) >= this->minIntervalMs) {
      userFunction();
      lastExecutionTime = currentTime;
    }
  };
}
