#include "pros/adi.hpp"
#include "Subsystem.h"

class Matchload : Subsystem {
    public: 
        Matchload matchload();
        Matchload matchload(int pistonPort);

        void out();
        void in();

        
        void setup() override; 
        void registerHandlers(EventLoop &eventLoop) override;

    private:
        pros::adi::Pneumatics pneumatic;
        int DEFAULT_PISTON_PORT = 5;


};