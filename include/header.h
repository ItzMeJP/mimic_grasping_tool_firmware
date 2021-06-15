/**\file main.cpp
 * \brief Grasping Mimic Firmware Declaration.
 *
 * @version 1.0.14062021
 * @author João Pedro Carvalho de Souza
 */

#include <Arduino.h>
#include <OneButton.h>
#include <SerialCommand.h>
#include <CompositeLED.h>

#define LED_RED 2
#define LED_GREEN 3
#define LED_BLUE 4
#define BUTTON_RECORD 8 //down
#define BUTTON_CTRL 7   //up
#define RELAY_1 6
#define RELAY_2 5
#define BAUD_RATE 115200

bool active_state = HIGH,
     desactive_state = LOW;

enum MSG_TYPE
{
  CONNECTION_STABILISHED_TWO_ALTERNATE_RELAYS = 100,
  CONNECTION_STABILISHED_ONE_RELAY,
  ACK,
  ERROR,
  SUCCESS,
  RESET,
  STATE_INIT = 500,
  STATE_RUNNING = 501,
  STATE_ACTIVE_GRIPPER = 502,
  STATE_SAVING = 503,
  STATE_ERROR = 504,
  STATE_CANCELLING = 505,
  STATE_SUCCESS = 506
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
  PARALLEL_PNEUMATIC_TWO_FINGER = 100,
  SINGLE_SUCTION_CUP = 101
};

SerialCommand sCmd;
void setMsgConnectionStabTwoAlternateRelays(),
     setMsgConnectionStabOneRelays(),
     setMsgACK(),
     setMsgERROR(),
     setMsgSUCCESS(),
     setMsgRESET(),
     setMsgStInit(),
     setMsgStRunning(),
     setMsgStActiveGripper(),
     setMsgStSaving(),
     setMsgStError(),
     setMsgStCancelling(),
     setMsgStSuccess();

void sendMSG(const char _msg[], uint32_t _delay);
void sendMSG(MSG_TYPE _msg, uint32_t _delay);

OneButton button_ctrl(BUTTON_CTRL, true),
    button_record(BUTTON_RECORD, true);

void callBackGripper();
void callBackSave();
void callbackDelete();

bool state_red = desactive_state,
     state_green = desactive_state,
     state_blue = desactive_state;

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
