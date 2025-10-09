#ifndef LONG_GOAL_AUTO_H
#define LONG_GOAL_AUTO_H

#include "lemlib/api.hpp" 

class LongGoalAuto {
public:
    LongGoalAuto(lemlib::Chassis &chassis);

    void run();

private: 
    lemlib::Chassis &chassis;
};

#endif