#pragma once
#include <functional>
#include <string>

class Command {
public:
  Command(const std::string& name, std::function<void()> executeFunc)
    : name(name), executeFunc(executeFunc) {}

  void execute() {
    executeFunc();
  }

  std::string name;

protected:
  std::function<void()> executeFunc;
};
