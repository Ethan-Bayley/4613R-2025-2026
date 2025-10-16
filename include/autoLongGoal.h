#ifndef AUTO_LONG_GOAL_H
#define AUTO_LONG_GOAL_H

#include "lemlib/api.hpp" 

class autoLongGoal {
public:
    autoLongGoal(lemlib::Chassis &chassis);

    void run();

private: 
    lemlib::Chassis &chassis;
};

#endif