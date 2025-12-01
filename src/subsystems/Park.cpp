// #include "subsystems/Park.h"
// #include "commands/ButtonCommand.h"
// #include <memory>

// Park::Park() : piston(DEFAULT_PISTON_PORT, false)
// {
// }

// Park::Park(int pistonPort) : piston(pistonPort, false)                          
// {
// }

// void Park::setup() {
//   piston.retract();
// }

// void Park::registerHandlers(EventLoop &eventLoop)
// {
//   eventLoop.registerCommand(
//     std::unique_ptr<Command>(new ButtonCommand(
//       eventLoop.controller,
//       pros::E_CONTROLLER_DIGITAL_A,
//       [this]() { toggle(); }
//     ))
//   );
// }

// void Park::toggle()
// {
//   if (piston.is_extended()) {
//     piston.retract();
//   } else {
//     piston.extend();
//   }

// }

