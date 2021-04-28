#include <CompositeLED.h>

namespace LEDFramework
{

    CompositeLED::CompositeLED(int _pin_a, int _pin_b, int _led_type, bool _state_type) : led_a_(), led_b_()
    {

        pin_a_ = _pin_a;
        pin_b_ = _pin_b;
        led_type_ = _led_type;

        led_a_.setParameters(_pin_a, _state_type);
        led_b_.setParameters(_pin_b, _state_type);
    }

    CompositeLED::CompositeLED(int _pin_a, int _pin_b, int _pin_c, int _led_type, bool _state_type) : led_a_(), led_b_(), led_c_()
    {

        led_type_ = _led_type;

        pin_a_ = _pin_a;
        pin_b_ = _pin_b;
        pin_c_ = _pin_c;

        led_a_.setParameters(_pin_a, _state_type);
        led_b_.setParameters(_pin_b, _state_type);
        led_c_.setParameters(_pin_c, _state_type);
    }

    CompositeLED::CompositeLED(int _pin_a, int _pin_b, int _pin_c, int _pin_d, int _led_type, bool _state_type) : led_a_(), led_b_(), led_c_(), led_d_()
    {

        led_type_ = _led_type;

        pin_a_ = _pin_a;
        pin_b_ = _pin_b;
        pin_c_ = _pin_c;
        pin_d_ = _pin_d;

        led_a_.setParameters(_pin_a, _state_type);
        led_b_.setParameters(_pin_b, _state_type);
        led_c_.setParameters(_pin_c, _state_type);
        led_d_.setParameters(_pin_d, _state_type);
    }

    CompositeLED::~CompositeLED(){};

    void CompositeLED::setColor(int _color)
    {
        first_ = true;
        switch (_color)
        {
        case COLOR::RED:
            setRed();
            break;
        case COLOR::GREEN:
            setGreen();
            break;
        case COLOR::BLUE:
            setBlue();
            break;
        case COLOR::YELLOW:
            setYellow();
            break;
        case COLOR::CYAN:
            setCyan();
            break;
        case COLOR::MAGENTA:
            setMagenta();
            break;
        case COLOR::WHITE:
            setWhite();
            break;
        case COLOR::BLANK:
            setBlank();
            break;
        default:
            break;
        }

        current_blink_color_ = _color;
    }

    void CompositeLED::setRed()
    {

        led_a_.setActive();
        led_b_.setDesactive();
        led_c_.setDesactive();
        led_d_.setDesactive();
    }

    void CompositeLED::setGreen()
    {

        led_a_.setDesactive();
        led_b_.setActive();
        led_c_.setDesactive();
        led_d_.setDesactive();
    }

    void CompositeLED::setBlue()
    {

        led_a_.setDesactive();
        led_b_.setDesactive();
        led_c_.setActive();
        led_d_.setDesactive();
    }

    void CompositeLED::setYellow()
    {
        if (led_type_ == LED_TYPE::RGY)
        {
            led_a_.setDesactive();
            led_b_.setDesactive();
            led_c_.setActive();
            led_d_.setDesactive();
        }
        else if (led_type_ == LED_TYPE::RGB)
        {
            led_a_.setActive();
            led_b_.setActive();
            led_c_.setDesactive();
            led_d_.setDesactive();
        }
    }

    void CompositeLED::setCyan()
    {

        led_a_.setDesactive();
        led_b_.setActive();
        led_c_.setActive();
        led_d_.setDesactive();
    }

    void CompositeLED::setMagenta()
    {

        led_a_.setActive();
        led_b_.setDesactive();
        led_c_.setActive();
        led_d_.setDesactive();
    }

    void CompositeLED::setWhite()
    {

        led_a_.setActive();
        led_b_.setActive();
        led_c_.setActive();
        led_d_.setActive();
    }

    void CompositeLED::setBlank()
    {

        led_a_.setDesactive();
        led_b_.setDesactive();
        led_c_.setDesactive();
        led_d_.setDesactive();
    }

    void CompositeLED::setBlink(int _color, int _period_ms)
    {
        if (first_ || (current_blink_color_ != _color))
        {
            led_a_.setDesactive();
            led_b_.setDesactive();
            led_c_.setDesactive();
            led_d_.setDesactive();
            first_ = false;
        }

        led_a_.setBlink(_period_ms);
        led_b_.setBlink(_period_ms);
        led_c_.setBlink(_period_ms);
        led_d_.setBlink(_period_ms);

        current_blink_color_ = _color;
    }

    void CompositeLED::tick()
    {

        switch (current_blink_color_)
        {
        case COLOR::RED:
            setTickRed();
            break;
        case COLOR::GREEN:
            setTickGreen();
            break;
        case COLOR::BLUE:
            setTickBlue();
            break;
        case COLOR::YELLOW:
            setTickYellow();
            break;
        case COLOR::CYAN:
            setTickCyan();
            break;
        case COLOR::MAGENTA:
            setTickMagenta();
            break;
        case COLOR::WHITE:
            setTickWhite();
            break;
        default:
            break;
        }
    }

    void CompositeLED::setTickRed()
    {
        led_a_.tick();
    }

    void CompositeLED::setTickGreen()
    {
        led_b_.tick();
    }

    void CompositeLED::setTickBlue()
    {
        led_c_.tick();
    }

    void CompositeLED::setTickYellow()
    {
        if (led_type_ == LED_TYPE::RGY)
        {
            led_c_.tick();
        }
        else if (led_type_ == LED_TYPE::RGB)
        {
            led_a_.tick();
            led_b_.tick();
        }
    }

    void CompositeLED::setTickCyan()
    {
        led_b_.tick();
        led_c_.tick();
    }

    void CompositeLED::setTickMagenta()
    {
        led_a_.tick();
        led_c_.tick();
    }

    void CompositeLED::setTickWhite()
    {
        led_a_.tick();
        led_b_.tick();
        led_c_.tick();
        led_d_.tick();
    }

} //endnamespace