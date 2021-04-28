#include <Led.h>

namespace LEDFramework
{
    LED::LED(){};

    LED::LED(int _pin, bool _state_type)
    {
        setParameters(_pin, _state_type);
        setState(desactive_state_);
    }

    LED::~LED(){};

    void LED::setParameters(int _pin, bool _state_type)
    {
        pin_ = _pin;

        if (_state_type == LED::ACTIVE_HIGH)
        {
            active_state_ = HIGH;
            desactive_state_ = LOW;
        }
        else
        {
            active_state_ = LOW;
            desactive_state_ = HIGH;
        }

        pinMode(pin_, OUTPUT);
    }

    void LED::setState(bool _state)
    {
        setState(pin_, _state, current_state_);
    }

    void LED::setState(int _pin, bool _state, bool &_currentState)
    {
        digitalWrite(_pin, _state);
        _currentState = _state;
    }

    void LED::setActive()
    {
        period_ms_ = -1;
        first_ = true;
        setState(active_state_);
    }

    void LED::setDesactive()
    {
        period_ms_ = -1;
        first_ = true;
        setState(desactive_state_);
    }

    bool LED::getCurrentState()
    {
        return current_state_;
    }

    void LED::setBlink(int _period_ms)
    {

        if (first_)
        {
            last_time_ = millis();
            first_ = false;
        }
        period_ms_ = _period_ms;
    }

    void LED::tick()
    {

        if (period_ms_ < 0)
            return;
        else
        {
            now_ = millis();

            if (now_ - last_time_ > period_ms_)
            {
                setState(!getCurrentState());
                last_time_ = millis();
            }
        }
    }

} //endnamespace