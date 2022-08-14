#pragma once
#include "..\Engine\Component.h" 

class Gravity : public Component {
public:
    Gravity(double value);
    double get_value();
private:
    double gravity;
};


