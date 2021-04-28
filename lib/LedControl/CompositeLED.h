#include <Arduino.h>
#include <Led.h>

#pragma once

#ifndef COMPOSITE_LED_H
#define COMPOSITE_LED_H

namespace LEDFramework
{
    class CompositeLED
    {
    public:
        enum LED_TYPE
        {
            RGB,
            RGY
            //RGBW, TODO: improve the range of LED types. SetColors need changes
            //RB,
            //RG,
            //GB
        };

        enum COLOR
        {
            RED,
            GREEN,
            BLUE,
            YELLOW,
            CYAN,
            MAGENTA,
            ORANGE,
            WHITE,
            BLANK
        };

        CompositeLED(int _pin_a, int _pin_b, int _led_type, bool _state_type);
        CompositeLED(int _pin_a, int _pin_b, int _pin_c, int _led_type, bool _state_type);
        CompositeLED(int _pin_a, int _pin_b, int _pin_c, int _pin_d,int _led_type,  bool _state_type);

        ~CompositeLED();

        void setColor(int _color);

        void setBlink(int _color, int _period_ms);

        void tick();

    protected:
        int pin_a_,
            pin_b_,
            pin_c_,
            pin_d_,
            led_type_,
            current_blink_color_;

        bool first_ = true;

        unsigned long last_time_, now_;

        LED led_a_,
            led_b_,
            led_c_,
            led_d_;

        void setRed(),
            setGreen(),
            setYellow(),
            setBlue(),
            setCyan(),
            setMagenta(),
            setWhite(),
            setBlank();

       void setTickRed(),
            setTickGreen(),
            setTickYellow(),
            setTickBlue(),
            setTickCyan(),
            setTickMagenta(),
            setTickWhite();
            
        void setActive();
        void setDesactive();
    };
}

#endif //COMPOSITE_LED_H