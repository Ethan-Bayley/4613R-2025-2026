#ifndef AUTO_SKILLS_H
#define AUTO_SKILLS_H

#include "lemlib/chassis/chassis.hpp"

class AutoSkills {
public: 
    AutoSkills(lemlib::Chassis &chassis);

    void run();

private: 
    lemlib::Chassis &chassis;
};

#endif