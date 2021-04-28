
/**\file main.cpp
 * \brief Grasping Mimic Firmware Implementation.
 *
 * @version 1.0.12042021
 * @author João Pedro Carvalho de Souza
 */

#include<header.h>

LEDFramework::CompositeLED output_led(LED_RED, LED_GREEN, LED_BLUE,LEDFramework::CompositeLED::RGB, LEDFramework::LED::ACTIVE_LOW);

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

  output_led.setBlink(LEDFramework::CompositeLED::YELLOW,500);
  //blinkLED(LED_COLOR::YELLOW);
  turnOffRelays();

  while (current_msg_ != MSG_TYPE::CONNECTION_STABILISHED_ONE_RELAY && current_msg_ != MSG_TYPE::CONNECTION_STABILISHED_TWO_ALTERNATE_RELAYS)
  {
    output_led.tick();
    sCmd.readSerial();
    sendMSG("Waiting for server connection.", 1000);
  }

  init_time = millis();
  //blinkLED(LED_COLOR::GREEN);
  output_led.setBlink(LEDFramework::CompositeLED::GREEN,250);

  while (millis() - init_time < 2000)
  {
    output_led.tick();
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
  output_led.setColor(LEDFramework::CompositeLED::WHITE);
  //setLED(LED_COLOR::WHITE);
  //End: State's 0utput

  while (true)
  {
    output_led.tick();
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
  
  output_led.setColor(LEDFramework::CompositeLED::CYAN);

  //setLED(LED_COLOR::CYAN);
  //End: State's 0utput

  while (true)
  {
    output_led.tick();
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
  
  //blinkLED(LED_COLOR::BLUE);
  output_led.setBlink(LEDFramework::CompositeLED::BLUE,500);

  //End: State's 0utput

  while (true)
  {
    output_led.tick();
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
  //blinkLED(LED_COLOR::RED);
  output_led.setBlink(LEDFramework::CompositeLED::RED,500);

  //End: State's 0utput

  while (true)
  {
    output_led.tick();
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
  output_led.setColor(LEDFramework::CompositeLED::GREEN);

  //setLED(LED_COLOR::GREEN);
  //End: State's 0utput

  while (true)
  {
    output_led.tick();
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
  //blinkLED(LED_COLOR::MAGENTA);
  output_led.setBlink(LEDFramework::CompositeLED::MAGENTA,250);
  //End: State's 0utput

  while (true)
  {
    output_led.tick();
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
      //blinkLED(LED_COLOR::GREEN);
      output_led.setBlink(LEDFramework::CompositeLED::GREEN,250);
      init_time = millis();
      while (millis() - init_time < 2000)
      {
        output_led.tick();
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
