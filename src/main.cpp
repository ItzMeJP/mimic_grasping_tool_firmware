
/**\file main.cpp
 * \brief Grasping Mimic Firmware Implementation.
 *
 * @version 1.0.27032021
 * @author João Pedro Carvalho de Souza
 */

#include<header.h>

/// <summary>
/// Update state LED based on global variables
/// </summary>
/// <param name="outcome"> message.</param>
void updateLEDsState()
{
  digitalWrite(LED_RED, state_red);
  digitalWrite(LED_GREEN, state_green);
  digitalWrite(LED_BLUE, state_blue);
}

/// <summary>
/// Red Led Blink Callback
/// </summary>
void callbackRedBlink()
{
  state_red = !state_red;
  state_green = desactive_state;
  state_blue = desactive_state;
  updateLEDsState();
}

/// <summary>
/// Green Led Blink Callback
/// </summary>
void callbackGreenBlink()
{
  state_red = desactive_state;
  state_green = !state_green;
  state_blue = desactive_state;
  updateLEDsState();
}

/// <summary>
/// Blue Led Blink Callback
/// </summary>
void callbackBlueBlink()
{
  state_red = desactive_state;
  state_green = desactive_state;
  state_blue = !state_blue;
  updateLEDsState();
}

/// <summary>
/// White Led Blink Callback
/// </summary>
void callbackWhiteBlink()
{
  state_red = !state_green;
  state_green = !state_green;
  state_blue = !state_blue;
  updateLEDsState();
}

/// <summary>
/// Cyan Led Blink Callback
/// </summary>
void callbackCyanBlink()
{
  state_red = desactive_state;
  state_green = !state_green;
  state_blue = !state_blue;
  updateLEDsState();
}

/// <summary>
/// Yellow Led Blink Callback
/// </summary>
void callbackYellowBlink()
{
  state_red = !state_red;
  state_green = !state_green;
  state_blue = desactive_state;
  updateLEDsState();
}

/// <summary>
/// Magenta Led Blink Callback
/// </summary>
void callbackMagentaBlink()
{
  state_red = !state_red;
  state_green = desactive_state;
  state_blue = !state_blue;
  updateLEDsState();
}

/// <summary>
/// Define the blinker scheduler. Each task is associated with a color blink callback. Since the taskscheduler library does not accept parameters, global variables were used. 
/// Task scheduler library is trick when performing Oriented Object Programming (or multifile association), thus no OP was not used in ths firmware.
/// TODO: improve this library. By not keep it simple =)
/// </summary>
void defineBlinkScheduler()
{

  task_blink_red_.setInterval(250);
  task_blink_red_.setIterations(TASK_FOREVER);
  task_blink_red_.setCallback(&callbackRedBlink);

  task_blink_green_.setInterval(250);
  task_blink_green_.setIterations(TASK_FOREVER);
  task_blink_green_.setCallback(&callbackGreenBlink);

  task_blink_blue_.setInterval(250);
  task_blink_blue_.setIterations(TASK_FOREVER);
  task_blink_blue_.setCallback(&callbackBlueBlink);

  task_blink_white_.setInterval(250);
  task_blink_white_.setIterations(TASK_FOREVER);
  task_blink_white_.setCallback(&callbackWhiteBlink);

  task_blink_yellow_.setInterval(250);
  task_blink_yellow_.setIterations(TASK_FOREVER);
  task_blink_yellow_.setCallback(&callbackYellowBlink);

  task_blink_cyan_.setInterval(250);
  task_blink_cyan_.setIterations(TASK_FOREVER);
  task_blink_cyan_.setCallback(&callbackCyanBlink);

  task_blink_magenta_.setInterval(250);
  task_blink_magenta_.setIterations(TASK_FOREVER);
  task_blink_magenta_.setCallback(&callbackMagentaBlink);

  runner_.init();

  runner_.addTask(task_blink_red_);
  runner_.addTask(task_blink_green_);
  runner_.addTask(task_blink_blue_);
  runner_.addTask(task_blink_white_);
  runner_.addTask(task_blink_yellow_);
  runner_.addTask(task_blink_cyan_);
  runner_.addTask(task_blink_magenta_);

  updateLEDsState();

  runner_.disableAll();
}

/// <summary>
/// Set current message (provide by SerialCommand library) with code 99 (see enum MSG_TYPE)
/// </summary>
void setMsg99()
{
  current_msg_ = MSG_TYPE::CONNECTION_STABILISHED_TWO_ALTERNATE_RELAYS;
}

/// <summary>
/// Set current message (provide by SerialCommand library) with code 100 (see enum MSG_TYPE)
/// </summary>
void setMsg100()
{
  current_msg_ = MSG_TYPE::CONNECTION_STABILISHED_ONE_RELAY;
}

/// <summary>
/// Set current message (provide by SerialCommand library) with code 101 (see enum MSG_TYPE)
/// </summary>
void setMsg101()
{
  current_msg_ = MSG_TYPE::ERROR;
  error_cmd_ = true;
}

/// <summary>
/// Set current message (provide by SerialCommand library) with code 102 (see enum MSG_TYPE)
/// </summary>
void setMsg102()
{
  current_msg_ = MSG_TYPE::SUCCESS;
  success_cmd_ = true;
}

/// <summary>
/// Set current message (provide by SerialCommand library) with code 103 (see enum MSG_TYPE)
/// </summary>
void setMsg103()
{
  current_msg_ = MSG_TYPE::RESET;
  save_cmd_ = false,
  relay_cmd_ = false,
  delete_cmd_ = false;
  error_cmd_ = false;
  success_cmd_ = false;
  reset_cmd_ = true;
}

/// <summary>
/// Callback to set the global variable relay_cmd_
/// </summary>
void callBackGripper()
{
  relay_cmd_ = !relay_cmd_;
}

/// <summary>
/// Callback to set the global variable save_cmd_
/// </summary>
void callBackSave()
{
  save_cmd_ = true;
}

/// <summary>
/// Callback to set the global variable delete_cmd_
/// </summary>
void callbackDelete()
{
  delete_cmd_ = true;
}

/// <summary>
/// Request a led color to blink.
/// </summary>
/// <param name="_led_color"> Define which color will blink. Check enum LED_COLOR </param>
void blinkLED(int _led_color)
{

  setLED(LED_COLOR::BLANK);

  switch (_led_color)
  {
  case LED_COLOR::RED:
    task_blink_red_.enableIfNot();
    task_blink_green_.disable();
    task_blink_blue_.disable();
    task_blink_cyan_.disable();
    task_blink_magenta_.disable();
    task_blink_yellow_.disable();
    task_blink_white_.disable();
    break;
  case LED_COLOR::GREEN:
    task_blink_red_.disable();
    task_blink_green_.enableIfNot();
    task_blink_blue_.disable();
    task_blink_cyan_.disable();
    task_blink_magenta_.disable();
    task_blink_yellow_.disable();
    task_blink_white_.disable();
    break;
  case LED_COLOR::BLUE:
    task_blink_red_.disable();
    task_blink_green_.disable();
    task_blink_blue_.enableIfNot();
    task_blink_cyan_.disable();
    task_blink_magenta_.disable();
    task_blink_yellow_.disable();
    task_blink_white_.disable();
    break;
  case LED_COLOR::YELLOW:
    task_blink_red_.disable();
    task_blink_green_.disable();
    task_blink_blue_.disable();
    task_blink_cyan_.disable();
    task_blink_magenta_.disable();
    task_blink_yellow_.enableIfNot();
    task_blink_white_.disable();

    break;
  case LED_COLOR::CYAN:
    task_blink_red_.disable();
    task_blink_green_.disable();
    task_blink_blue_.disable();
    task_blink_cyan_.enableIfNot();
    task_blink_magenta_.disable();
    task_blink_yellow_.disable();
    task_blink_white_.disable();

    break;
  case LED_COLOR::MAGENTA:
    task_blink_red_.disable();
    task_blink_green_.disable();
    task_blink_blue_.disable();
    task_blink_cyan_.disable();
    task_blink_magenta_.enableIfNot();
    task_blink_yellow_.disable();
    task_blink_white_.disable();
    ;

    break;
  case LED_COLOR::WHITE:
    task_blink_red_.disable();
    task_blink_green_.disable();
    task_blink_blue_.disable();
    task_blink_cyan_.disable();
    task_blink_magenta_.disable();
    task_blink_yellow_.disable();
    task_blink_white_.enableIfNot();

    break;

  default:
    break;
  }
}

/// <summary>
/// Request a led color to turn ON.
/// </summary>
/// <param name="_led_color"> Define which color will be activated. Check enum LED_COLOR. Set color BLANK to turn it off </param>
void setLED(int _led_color)
{
  runner_.disableAll();

  switch (_led_color)
  {
  case LED_COLOR::RED:
    state_red = active_state;
    state_green = desactive_state;
    state_blue = desactive_state;
    updateLEDsState();
    break;
  case LED_COLOR::GREEN:
    state_red = desactive_state;
    state_green = active_state;
    state_blue = desactive_state;
    updateLEDsState();
    break;
  case LED_COLOR::BLUE:
    state_red = desactive_state;
    state_green = desactive_state;
    state_blue = active_state;
    updateLEDsState();
    break;
  case LED_COLOR::WHITE:
    state_red = active_state;
    state_green = active_state;
    state_blue = active_state;
    updateLEDsState();
    break;
  case LED_COLOR::CYAN:
    state_red = desactive_state;
    state_green = active_state;
    state_blue = active_state;
    updateLEDsState();
    break;
  case LED_COLOR::MAGENTA:
    state_red = active_state;
    state_green = desactive_state;
    state_blue = active_state;
    updateLEDsState();
    break;
  case LED_COLOR::YELLOW:
    state_red = active_state;
    state_green = active_state;
    state_blue = desactive_state;
    updateLEDsState();
    break;
  case LED_COLOR::BLANK:
    state_red = desactive_state;
    state_green = desactive_state;
    state_blue = desactive_state;
    updateLEDsState();
    break;

  default:
    break;
  }
}

/// <summary>
/// Set relays states. It will run according to the global variable state_type_ .Relays are related to pneumatic grippers. Check enum GRIPPER_TYPE.
/// </summary>
/// <param name="_state"> State of gripper (active or not) </param>
void setRelays(bool _state)
{

  if (relay_type_ == GRIPPER_TYPE::SINGLE_SUCTION_CUP)
  {

    digitalWrite(RELAY_1, _state);
  }
  else if (relay_type_ == GRIPPER_TYPE::PARALLEL_PNEUMATIC_TWO_FINGER)
  {

    digitalWrite(RELAY_1, !_state);
    digitalWrite(RELAY_2, _state);
  }
}

/// <summary>
/// Set all relays to inactive. 
/// </summary>
void turnOffRelays()
{
    digitalWrite(RELAY_1, 0);
    digitalWrite(RELAY_2, 0);

}

/// <summary>
/// Send a serial message with timestamp header. Usefull to print info messages Terminator is \r
/// </summary>
/// <param name="_msg"> message to pub in serial </param>
/// <param name="_delay"> delay time to send the msg in ms </param>
void sendMSG(const char _msg[], uint32_t _delay)
{
  static long int init_time = millis();
  if (millis() - init_time > _delay)
  {
    Serial.print(millis());
    Serial.print(": ");
    Serial.println(_msg);
    init_time = millis();
  }
}

/// <summary>
/// Send a serial message code without header. Usefull to request data from server. Terminator is \r
/// </summary>
/// <param name="_msg"> code message to pub in serial </param>
/// <param name="_delay"> delay time to send the msg in ms </param>
void sendMSG(MSG_TYPE _msg, uint32_t _delay)
{
  static long int init_time = millis();
  if (millis() - init_time > _delay)
  {
    Serial.println(_msg);
    init_time = millis();
  }
}

/// <summary>
/// Waiting firmware to connect to the server
/// </summary>
void waitForServerConnection()
{
  long int init_time;

  blinkLED(LED_COLOR::YELLOW);
  turnOffRelays();

  while (current_msg_ != MSG_TYPE::CONNECTION_STABILISHED_ONE_RELAY && current_msg_ != MSG_TYPE::CONNECTION_STABILISHED_TWO_ALTERNATE_RELAYS)
  {
    runner_.execute();
    sCmd.readSerial();
    sendMSG("Waiting for server connection.", 1000);
  }

  init_time = millis();
  blinkLED(LED_COLOR::GREEN);

  while (millis() - init_time < 2000)
  {
    runner_.execute();
    sCmd.readSerial();
  }

  relay_type_ = current_msg_;
  Serial.print("Connection Stablished. Relay mode: ");
  Serial.println(relay_type_);

  reset_cmd_ = false;
}

/// <summary>
/// FSM init state
/// </summary>
void initState()
{
  Serial.println("Current State >> Init State.");

  //Start: State's 0utput
  setRelays(LOW); // Suction off or 2F opened
  save_cmd_ = false,
  relay_cmd_ = false,
  delete_cmd_ = false;
  error_cmd_ = false;
  success_cmd_ = false;
  setLED(LED_COLOR::WHITE);
  //End: State's 0utput

  while (true)
  {
    runner_.execute();
    sCmd.readSerial();
    button_ctrl.tick();
    button_record.tick();

    //Start: Transitions Verification
    if (reset_cmd_)
    {
      Serial.println("Reseting request.");
      loop();
    }

    else if (relay_cmd_)
    {
      relayOnState();
    }
    //end: Transitions Verification
  }
}

/// <summary>
/// FSM relayOn
/// </summary>
void relayOnState()
{
  Serial.println("Current State >> Relay ON.");

  //Start: State's 0utput
  setRelays(HIGH); // Suction ON or 2F CLOSED
  save_cmd_ = false,
  delete_cmd_ = false;
  error_cmd_ = false;
  success_cmd_ = false;
  setLED(LED_COLOR::CYAN);
  //End: State's 0utput

  while (true)
  {
    runner_.execute();
    sCmd.readSerial();
    button_ctrl.tick();
    button_record.tick();

    if (reset_cmd_)
    {
      Serial.println("Reseting request.");
      loop();
    }

    else if (!relay_cmd_)
    {
      initState();
    }
    else if (save_cmd_)
    {
      Serial.println("Saving request.");
      saveState();
    }
  }
}

/// <summary>
/// FSM save state
/// </summary>
void saveState()
{
  Serial.println("Current State >> Save State.");

  //Start: State's 0utput
  setRelays(HIGH); // Suction ON or 2F CLOSED
  save_cmd_ = false,
  delete_cmd_ = false;
  error_cmd_ = false;
  success_cmd_ = false;
  blinkLED(LED_COLOR::BLUE);
  //End: State's 0utput

  while (true)
  {
    runner_.execute();
    sCmd.readSerial();
    button_ctrl.tick();
    button_record.tick();

    sendMSG(MSG_TYPE::SAVE,500);

    if (reset_cmd_)
    {
      Serial.println("Reseting request.");
      loop();
    }

    else if (error_cmd_)
    {
      Serial.println("Unable to record grasping.");
      errorState();
    }

    else if (success_cmd_)
    {
      Serial.println("Grasping saved with success.");
      successState();
    }
  }
}

/// <summary>
/// FSM error state
/// </summary>
void errorState()
{
  Serial.println("Current State >> Error State.");

  //Start: State's 0utput
  setRelays(HIGH); // Suction ON or 2F CLOSED
  save_cmd_ = false,
  delete_cmd_ = false;
  error_cmd_ = false;
  success_cmd_ = false;
  blinkLED(LED_COLOR::RED);
  //End: State's 0utput

  while (true)
  {
    runner_.execute();
    sCmd.readSerial();
    button_ctrl.tick();
    button_record.tick();

    if (reset_cmd_)
    {
      Serial.println("Reseting request.");
      loop();
    }

    else if (!relay_cmd_)
    {
      initState();
    }
  }
}

/// <summary>
/// FSM success state
/// </summary>
void successState()
{
  Serial.println("Current State >> Success State.");

  //Start: State's 0utput
  setRelays(HIGH); // Suction ON or 2F CLOSED
  save_cmd_ = false,
  delete_cmd_ = false;
  error_cmd_ = false;
  success_cmd_ = false;
  setLED(LED_COLOR::GREEN);
  //End: State's 0utput

  while (true)
  {
    runner_.execute();
    sCmd.readSerial();
    button_ctrl.tick();
    button_record.tick();

    if (reset_cmd_)
    {
      Serial.println("Reseting request.");
      loop();
    }

    else if (delete_cmd_)
    {
      cancelState();
    }
    else if (!relay_cmd_)
    {
      initState();
    }
  }
}

/// <summary>
/// FSM cancel state
/// </summary>
void cancelState()
{

  long int init_time;
  Serial.println("Current State >> Cancel State.");

  //Start: State's 0utput
  setRelays(HIGH); // Suction ON or 2F CLOSED
  save_cmd_ = false,
  delete_cmd_ = false;
  error_cmd_ = false;
  success_cmd_ = false;
  blinkLED(LED_COLOR::MAGENTA);
  //End: State's 0utput

  while (true)
  {
    runner_.execute();
    sCmd.readSerial();
    button_ctrl.tick();
    button_record.tick();

    sendMSG(MSG_TYPE::REMOVE_LAST_SAVE,500);

    if (reset_cmd_)
    {
      Serial.println("Reseting request.");
      loop();
    }

    else if (success_cmd_)
    {
      Serial.println("Last save canceled.");
      blinkLED(LED_COLOR::GREEN);
      init_time = millis();
      while (millis() - init_time < 2000)
      {
        runner_.execute();
      }
      relayOnState();
    }
  }
}

/// <summary>
/// Arduino setup function
/// </summary>
void setup()
{
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(BUTTON_CTRL, INPUT);
  pinMode(BUTTON_RECORD, INPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  Serial.begin(BAUD_RATE);

  // SerialCommand only support functions without parameters (TODO: improve this lib to fit Oriented Programming Requisites)
  sCmd.addCommand("99", setMsg99);
  sCmd.addCommand("100", setMsg100);
  sCmd.addCommand("101", setMsg101);
  sCmd.addCommand("102", setMsg102);
  sCmd.addCommand("103", setMsg103);

  // OneButton only support functions without parameters (TODO: improve this lib to fit Oriented Programming Requisites)
  button_ctrl.attachClick(callBackGripper);
  button_record.attachClick(callBackSave);
  button_record.attachLongPressStop(callbackDelete);

  defineBlinkScheduler();
}

/// <summary>
/// Arduino run function
/// </summary>
void loop()
{
  current_msg_ = -1;
  waitForServerConnection();
  initState();
}
