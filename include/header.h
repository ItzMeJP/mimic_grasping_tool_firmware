/**\file main.cpp
 * \brief Grasping Mimic Firmware Declaration.
 *
 * @version 1.0.27032021
 * @author João Pedro Carvalho de Souza
 */

#include <Arduino.h>
#include <OneButton.h>
#include <SerialCommand.h>
#include <TaskScheduler.h>

#define LED_RED 2
#define LED_GREEN 3
#define LED_BLUE 5
#define BUTTON_RECORD 7 //down
#define BUTTON_CTRL 8   //up
#define RELAY_1 4
#define RELAY_2 9
#define BAUD_RATE 115200

bool active_state = LOW,
     desactive_state = HIGH;

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

enum GRIPPER_TYPE{
  PARALLEL_PNEUMATIC_TWO_FINGER = 99,
  SINGLE_SUCTION_CUP = 100
};

SerialCommand sCmd;
void setMsg99(),
     setMsg100(),
     setMsg101(),
     setMsg102(),
     setMsg103();

void sendMSG(const char _msg[], uint32_t _delay);
void sendMSG(MSG_TYPE _msg, uint32_t _delay);

OneButton button_ctrl(BUTTON_CTRL, true),
    button_record(BUTTON_RECORD, true);

void callBackGripper();
void callBackSave();
void callbackDelete();

Scheduler runner_;
Task task_blink_red_,
    task_blink_blue_,
    task_blink_green_,
    task_blink_white_,
    task_blink_yellow_,
    task_blink_cyan_,
    task_blink_magenta_;

bool state_red = desactive_state,
     state_green = desactive_state,
     state_blue = desactive_state;

void updateLEDsState();
void callbackRedBlink();
void callbackGreenBlink();
void callbackBlueBlink();
void callbackWhiteBlink();
void callbackCyanBlink();
void callbackYellowBlink();
void callbackMagentaBlink();
void blinkLED(int _led_color);
void setLED(int _led_color);

void waitForServerConnection();
void initState();
void relayOnState();
void saveState();
void errorState();
void successState();
void cancelState();

void setRelays(bool _state);
void turnOffRelays();

int current_msg_,
    relay_type_;

bool save_cmd_,
    delete_cmd_,
    error_cmd_,
    success_cmd_,
    reset_cmd_,
    relay_cmd_;
