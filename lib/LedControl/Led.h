#include <Arduino.h>

#pragma once

#ifndef LED_H
#define LED_H

namespace LEDFramework {
    class LED {
    public:

        enum STATE_TYPE{
            ACTIVE_LOW=0,
            ACTIVE_HIGH=1
        };

        LED();
        LED(int _pin, bool _state_type);
        ~LED();

        void setParameters(int _pin, bool _state_type);
        void setBlink(int _period_ms);
        void setActive();
        void setDesactive();
        bool getCurrentState();
        void tick();

    protected:
        int pin_;
        bool active_state_,
             desactive_state_,
             current_state_,
             first_=true;

        unsigned long last_time_, now_;

        int period_ms_=-1;
        
        void setState(bool _state);
        void setState(int _pin, bool _state, bool &_currentState);

    };
}

#endif //LED_H