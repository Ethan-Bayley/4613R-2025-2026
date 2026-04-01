#include "pros/adi.hpp"
#include "Subsystem.h"

class Wing : Subsystem {
    public: 
        Wing wing();
        Wing wing(int pistonPort);

        void extend();
        void retract();

        
        void setup() override; 
        void registerHandlers(EventLoop &eventLoop) override;

    private:
        pros::adi::Pneumatics pneumatic;
        int DEFAULT_PISTON_PORT = 5;


};