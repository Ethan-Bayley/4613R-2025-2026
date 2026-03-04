#pragma once

class Autonomous {
public:
    virtual ~Autonomous() = default;

    // Main autonomous routine
    virtual void run() = 0;
};
