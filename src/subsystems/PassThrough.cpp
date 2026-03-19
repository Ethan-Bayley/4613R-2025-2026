
#include "pros/motors.h"
#include "subsystems/Passthrough.h"



Passthrough::Passthrough() : 
    intakeMotor(INTAKE_MOTOR_PORT, pros::MotorGearset::blue),
    midGoalMotor(MIDGOAL_MOTOR_PORT, pros::MotorGearset::blue),
    longGoalMotor(LONGGOAL_MOTOR_PORT, pros::MotorGearset::blue)

{
}

void Passthrough::registerHandlers(EventLoop &eventLoop)
{
  // Intake
  eventLoop.registerCommand(
    std::unique_ptr<Command>(new ButtonCommand(
      eventLoop.controller,
      pros::E_CONTROLLER_DIGITAL_R1,
      [this]() { intake(); },
      [this]() { stop(); }
    ))
  );


  // Outtake
  eventLoop.registerCommand(
    std::unique_ptr<Command>(new ButtonCommand(
      eventLoop.controller,
      pros::E_CONTROLLER_DIGITAL_R2,
      [this]() { outtake(); },
      [this]() { stop(); }
    ))
  );

  // Score Long
  eventLoop.registerCommand(
    std::unique_ptr<Command>(new ButtonCommand(
      eventLoop.controller,
      pros::E_CONTROLLER_DIGITAL_L1,
      [this]() { scoreLong(); },
      [this]() { stop(); }
    ))
  );

  // Score Middle
  eventLoop.registerCommand(
    std::unique_ptr<Command>(new ButtonCommand(
      eventLoop.controller,
      pros::E_CONTROLLER_DIGITAL_L2,
      [this]() { scoreMiddle(); },
      [this]() { stop();}
    ))
  );


}



void Passthrough::intake(){
    intakeMotor.move_voltage(12000);
}

void Passthrough::outtake(){
    intakeMotor.move_voltage(-12000);
}

void Passthrough::stop(){
    intakeMotor.move_voltage(0);
}

void Passthrough::scoreMiddle(){
    intakeMotor.move_voltage(12000);
    midGoalMotor.move_voltage(12000);
}

void Passthrough::scoreLong(){
    intakeMotor.move_voltage(12000);
    longGoalMotor.move_voltage(12000);
}
