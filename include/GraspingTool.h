#include <Arduino.h>
#include <OneButton.h>
#include <SerialCommand.h>

#define LED_RED 2
#define LED_GREEN 3
#define LED_BLUE 5
#define BUTTON_RECORD 7 //down
#define BUTTON_CTRL 8   //up
#define RELAY_1 4
#define RELAY_2 9
#define BAUD_RATE 115200

#define MSG_SIZE 2

//#pragma once

#ifndef GRASPINGTOOL_H
#define GRASPINGTOOL_H

namespace grasping_firmware_tool
{

    class GraspingTool
    {
    public:
        GraspingTool();
        ~GraspingTool();

        enum MSG_TYPE
        {
            CONNECTION_STABILISHED_TWO_ALTERNATE_RELAYS = 99,
            CONNECTION_STABILISHED_ONE_RELAY = 100,
            ERROR = 101,
            SUCCESS = 102,
            RESET = 103,
            SAVE = 104,
            REMOVE_LAST_SAVE = 105
        };

        enum LED_COLOR
        {
            RED,
            GREEN,
            BLUE,
            YELLOW,
            CYAN,
            MAGENTA,
            WHITE,
            BLANK
        };

        bool load();
        bool run();


    protected:
        SerialCommand sCmd;
        static void setMsg99(), 
                    setMsg100(),
                    setMsg101(),
                    setMsg102(),
                    setMsg103();

        OneButton button_ctrl, button_record;

        static void callBackGripper();
        static void callBackSave();
        static void callbackDelete();

        void blinkLED(int _led_color);
        void setLED(int _led_color);

        void initState();
        void relayOnState();
        void saveState();
        void errorState();
        void successState();
        void cancelState();

        void waitForServerConnection();
        void sendMSG(const char _msg[]);
        void sendMSG(MSG_TYPE _msg);
        void setRelays(bool _state);

    };

}
#endif //GRASPINGTOOL_H