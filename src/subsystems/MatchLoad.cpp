// #include "subsystems/MatchLoad.h"
// #include "commands/ButtonCommand.h"

// MatchLoad::MatchLoad() : piston1(DEFAULT_PISTON_1_PORT, false),
//                          piston2(DEFAULT_PISTON_2_PORT, true),
//                          isExtended(false)
// {
// }

// MatchLoad::MatchLoad(int pistonPort1, int pistonPort2) : piston1(pistonPort1, false),
//                                                          piston2(pistonPort2, true),
//                                                          isExtended(false)
// {
// }

// void MatchLoad::registerHandlers(EventLoop &eventLoop)
// {
//   eventLoop.registerCommand(
//     std::make_shared<ButtonCommand>(
//       eventLoop.controller,
//       pros::E_CONTROLLER_DIGITAL_B,
//       [this]() { toggle(); }
//     )
//   );
// }

// void MatchLoad::setup()
// {
// }

// void MatchLoad::update()
// {
// }

// void MatchLoad::extend()
// {
//   piston1.extend();
//   piston2.retract();
//   isExtended = true;
// }

// void MatchLoad::retract()
// {
//   piston1.retract();
//   piston2.extend();
//   isExtended = false;
// }

// void MatchLoad::toggle()
// {
//   if (isExtended){
//     retract();
//   }
//   else{
//     extend();
//   }

// }