
/**\file main.cpp
 * \brief Grasping Mimic Firmware Implementation.
 *
 * @version 2.0.13072022
 * @author João Pedro Carvalho de Souza
 */

#include <header.h>

LEDFramework::CompositeLED output_led(LED_RED, LED_GREEN, LED_BLUE, LEDFramework::CompositeLED::RGB, LEDFramework::LED::ACTIVE_LOW);

// TODO: SerialCommand only support functions without parameters (TODO: improve this lib to fit Oriented Programming Requisites)

/// <summary>
/// Set current message (provide by SerialCommand library) with code (see enum MSG_TYPE)
/// </summary>
void setMsgConnectionStabTwoAlternateRelays()
{
  current_msg_ = MSG_TYPE::CONNECTION_STABILISHED_TWO_ALTERNATE_RELAYS;
}

/// <summary>
/// Set current message (provide by SerialCommand library) with code (see enum MSG_TYPE)
/// </summary>
void setMsgConnectionStabOneRelays()
{
  current_msg_ = MSG_TYPE::CONNECTION_STABILISHED_ONE_RELAY;
}

/// <summary>
/// Set current message (provide by SerialCommand library) with code (see enum MSG_TYPE)
/// </summary>
void setMsgACK()
{
  current_msg_ = MSG_TYPE::ACK;
}

/// <summary>
/// Set current message (provide by SerialCommand library) with code (see enum MSG_TYPE)
/// </summary>
void setMsgERROR()
{
  current_msg_ = MSG_TYPE::ERROR;
  error_cmd_ = true;
}

/// <summary>
/// Set current message (provide by SerialCommand library) with code (see enum MSG_TYPE)
/// </summary>
void setMsgSUCCESS()
{
  current_msg_ = MSG_TYPE::SUCCESS;
  success_cmd_ = true;
}

/// <summary>
/// Set current message (provide by SerialCommand library) with code (see enum MSG_TYPE)
/// </summary>
void setMsgRESET()
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
/// Set current message (provide by SerialCommand library) with code (see enum MSG_TYPE)
/// </summary>
void setMsgSHUTDOWN()
{
  current_msg_ = MSG_TYPE::SHUTDOWN;
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

  if (relay_type_ == GRIPPER_TYPE::SCHMALZ_FOAM_SUCTION_CUP_FMSW_N10_76x22)
  {

    digitalWrite(RELAY_1, _state);
  }
  else if (relay_type_ == GRIPPER_TYPE::FESTO_2F_HGPC_16_A_30)
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
/// Send a single serial message with timestamp header. Usefull to print info messages Terminator is \r
/// </summary>
/// <param name="_msg"> message to pub in serial </param>
void sendMSG(String _msg)
{
  Serial.print(millis());
  Serial.print(": ");
  Serial.println(_msg);
}

/// <summary>
/// Send a single serial message with timestamp header. Usefull to print info messages Terminator is \r
/// </summary>
/// <param name="_msg"> message to pub in serial </param>
void sendMSG(int _msg)
{
  Serial.print(millis());
  Serial.print(": ");
  Serial.println(_msg);
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
/// Send a serial message with timestamp header. Usefull to print info messages Terminator is \r
/// </summary>
/// <param name="_msg"> message to pub in serial </param>
/// <param name="_delay"> delay time to send the msg in ms </param>
void sendMSG(String _msg, uint32_t _delay)
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
/// FSM init state
/// </summary>
void waitForServerConnection()
{
  long int init_time;

  output_led.setBlink(LEDFramework::CompositeLED::YELLOW, 500);
  //blinkLED(LED_COLOR::YELLOW);
  turnOffRelays();

  while ( current_msg_ != MSG_TYPE::CONNECTION_STABILISHED_ONE_RELAY 
       && current_msg_ != MSG_TYPE::CONNECTION_STABILISHED_TWO_ALTERNATE_RELAYS
       && current_msg_ != MSG_TYPE::SHUTDOWN)
  {
    output_led.tick();
    sCmd.readSerial();
    String s(MSG_TYPE::STATE_INIT);
    sendMSG("Waiting for server connection #" + s, 1000);
  }

  if(current_msg_ == MSG_TYPE::SHUTDOWN)
    return;

  init_time = millis();
  //blinkLED(LED_COLOR::GREEN);
  output_led.setBlink(LEDFramework::CompositeLED::GREEN, 250);

  while (millis() - init_time < 2000)
  {
    output_led.tick();
    sCmd.readSerial();
  }

  relay_type_ = current_msg_;
  String s(relay_type_);
  sendMSG("Connection Stablished. Relay mode: " + s);
  reset_cmd_ = false;
}

/// <summary>
/// FSM running state
/// </summary>
void initState()
{
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
    String s(MSG_TYPE::STATE_RUNNING);
    sendMSG("Running State #" + s, 1000);

    output_led.tick();
    sCmd.readSerial();
    button_ctrl.tick();
    button_record.tick();

    //Start: Transitions Verification
    if (reset_cmd_)
    {
      sendMSG("Reseting request.");
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
    String s(MSG_TYPE::STATE_ACTIVE_GRIPPER);
    sendMSG("Gripper Activated #" + s, 1000);

    output_led.tick();
    sCmd.readSerial();
    button_ctrl.tick();
    button_record.tick();

    if (reset_cmd_)
    {
      sendMSG("Reseting request.");
      loop();
    }

    else if (!relay_cmd_)
    {
      initState();
    }
    else if (save_cmd_)
    {
      sendMSG("Saving request.");
      saveState();
    }
  }
}

/// <summary>
/// FSM save state
/// </summary>
void saveState()
{
  //Start: State's 0utput
  setRelays(HIGH); // Suction ON or 2F CLOSED
  save_cmd_ = false,
  delete_cmd_ = false;
  error_cmd_ = false;
  success_cmd_ = false;

  //blinkLED(LED_COLOR::BLUE);
  output_led.setBlink(LEDFramework::CompositeLED::BLUE, 500);

  //End: State's 0utput

  long int init_save_state_time, current_save_state_time;

  init_save_state_time = millis();

  while (true)
  {
    String s(MSG_TYPE::STATE_SAVING);
    sendMSG("Save request state #" + s, 1000);

    output_led.tick();
    sCmd.readSerial();
    button_ctrl.tick();
    button_record.tick();

    if (reset_cmd_)
    {
      sendMSG("Reseting request.");
      loop();
    }

    else if (error_cmd_)
    {
      sendMSG("Unable to record grasping. Error CMD detected.");
      errorState();
    }

    else if (success_cmd_)
    {
      sendMSG("Grasping saved with success.");
      successState();
    }

    else if (current_save_state_time - init_save_state_time >= SAVE_STATE_TIMEOUT_MS)
    {
      sendMSG("Unable to record grasping. Timeout exceeded 6s.");
      errorState();
    }

    current_save_state_time = millis();

  }
}

/// <summary>
/// FSM error state
/// </summary>
void errorState()
{
  //Start: State's 0utput
  setRelays(HIGH); // Suction ON or 2F CLOSED
  save_cmd_ = false,
  delete_cmd_ = false;
  error_cmd_ = false;
  success_cmd_ = false;
  //blinkLED(LED_COLOR::RED);
  output_led.setBlink(LEDFramework::CompositeLED::RED, 500);

  //End: State's 0utput

  while (true)
  {
    String s(MSG_TYPE::STATE_ERROR);
    sendMSG("Error state #" + s, 1000);

    output_led.tick();
    sCmd.readSerial();
    button_ctrl.tick();
    button_record.tick();

    if (reset_cmd_)
    {
      sendMSG("Reseting request.");
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
    String s(MSG_TYPE::STATE_SUCCESS);
    sendMSG("Succes state #" + s, 1000);

    output_led.tick();
    sCmd.readSerial();
    button_ctrl.tick();
    button_record.tick();

    if (reset_cmd_)
    {
      sendMSG("Reseting request.");
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

  //Start: State's 0utput
  setRelays(HIGH); // Suction ON or 2F CLOSED
  save_cmd_ = false,
  delete_cmd_ = false;
  error_cmd_ = false;
  success_cmd_ = false;
  //blinkLED(LED_COLOR::MAGENTA);
  output_led.setBlink(LEDFramework::CompositeLED::MAGENTA, 250);
  //End: State's 0utput

  while (true)
  {
    String s(MSG_TYPE::STATE_CANCELLING);
    sendMSG("Cancelling request state #" + s, 1000);

    output_led.tick();
    sCmd.readSerial();
    button_ctrl.tick();
    button_record.tick();

    if (reset_cmd_)
    {
      sendMSG("Reseting request.");
      loop();
    }

    else if (success_cmd_)
    {
      Serial.println("Last save canceled.");
      //blinkLED(LED_COLOR::GREEN);
      output_led.setBlink(LEDFramework::CompositeLED::GREEN, 250);
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
  char buffer[5];
  itoa(MSG_TYPE::CONNECTION_STABILISHED_TWO_ALTERNATE_RELAYS, buffer, 10);
  sCmd.addCommand(buffer, setMsgConnectionStabTwoAlternateRelays);
  itoa(MSG_TYPE::CONNECTION_STABILISHED_ONE_RELAY, buffer, 10);
  sCmd.addCommand(buffer, setMsgConnectionStabOneRelays);
  itoa(MSG_TYPE::SUCCESS, buffer, 10);
  sCmd.addCommand(buffer, setMsgSUCCESS);
  itoa(MSG_TYPE::ACK, buffer, 10);
  sCmd.addCommand(buffer, setMsgACK);
  itoa(MSG_TYPE::ERROR, buffer, 10);
  sCmd.addCommand(buffer, setMsgERROR);
  itoa(MSG_TYPE::RESET, buffer, 10);
  sCmd.addCommand(buffer, setMsgRESET);
  itoa(MSG_TYPE::SHUTDOWN, buffer, 10);
  sCmd.addCommand(buffer, setMsgSHUTDOWN);  

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

  /*
  if(current_msg_ == MSG_TYPE::SHUTDOWN){   
    button_ctrl.reset();
    button_record.reset();
    output_led.setColor(LEDFramework::CompositeLED::BLANK);
    Serial.end();
    //wdt_enable(WDTO_2S); 
    exit(0);
  }
  */ // TODO: try to apply the software reset instead of pusshing arduino button to load new firmwares.
    
  initState();

}

