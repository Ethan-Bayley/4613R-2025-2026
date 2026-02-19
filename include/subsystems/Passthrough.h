#include "Subsystems.h"
#include "pros/motors.hpp"
#include "pros/adi.hpp"

class Passthrough : public Subsystems {

    Passthrough();
    Passthrough(
        int intakeMotorPort, int midGoalMotorPort, int longGoalMotorPort,
        int matchLoaderPistonPort, int lWingPistonPort, int rWingPistonPort, int odomPistonPort);

    void setup() override;
    void registerHandlers(EventLoop &eventLoop) override;


    void intake();
    void outtake();
    void scoreMiddle();
    void scoreLong();
    void stop();


private:
  static constexpr int INTAKE_MOTOR_PORT = 7;
  static constexpr int MIDGOAL_MOTOR_PORT = 20;
  static constexpr int LONGGOAL_MOTOR_PORT = 12;

  pros::Motor intakeMotor;
  pros::Motor midGoalMotor;
  pros::Motor longGoalMotor;


};
