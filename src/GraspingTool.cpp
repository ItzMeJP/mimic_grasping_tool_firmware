#include <GraspingTool.h>
#include <TaskScheduler.h>

namespace grasping_firmware_tool
{

    Scheduler runner_; // task scheduler cant work inside my class. Thus, everything related is place outside the class but inside the scope
                       //runner_fsm;

    Task task_blink_red_,
        task_blink_blue_,
        task_blink_green_,
        task_blink_white_,
        task_blink_yellow_,
        task_blink_cyan_,
        task_blink_magenta_;

    static int current_msg_,
        relay_type_;

    static bool save_cmd_,
        relay_cmd_,
        delete_cmd_,
        error_cmd_,
        success_cmd_,
        reset_cmd_;

    bool active_state = LOW,
         desactive_state = HIGH;

    static bool state_red = desactive_state;
    static bool state_green = desactive_state;
    static bool state_blue = desactive_state;

    void updateLEDsState()
    {

        digitalWrite(LED_RED, state_red);
        digitalWrite(LED_GREEN, state_green);
        digitalWrite(LED_BLUE, state_blue);
    }

    void callbackRedBlink()
    {
        state_red = !state_red;
        state_green = desactive_state;
        state_blue = desactive_state;
        updateLEDsState();
    }

    void callbackGreenBlink()
    {
        state_red = desactive_state;
        state_green = !state_green;
        state_blue = desactive_state;
        updateLEDsState();
    }

    void callbackBlueBlink()
    {
        state_red = desactive_state;
        state_green = desactive_state;
        state_blue = !state_blue;
        updateLEDsState();
    }

    void callbackWhiteBlink()
    {
        state_red = !state_green;
        state_green = !state_green;
        state_blue = !state_blue;
        updateLEDsState();
    }

    void callbackCyanBlink()
    {
        state_red = desactive_state;
        state_green = !state_green;
        state_blue = !state_blue;
        updateLEDsState();
    }

    void callbackYellowBlink()
    {
        state_red = !state_red;
        state_green = !state_green;
        state_blue = desactive_state;
        updateLEDsState();
    }

    void callbackMagentaBlink()
    {
        state_red = !state_red;
        state_green = desactive_state;
        state_blue = !state_blue;
        updateLEDsState();
    }

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

    GraspingTool::GraspingTool() : button_ctrl(BUTTON_CTRL, true), button_record(BUTTON_RECORD, true){};

    GraspingTool::~GraspingTool(){};

    void GraspingTool::setMsg99()
    {
        current_msg_ = GraspingTool::MSG_TYPE::CONNECTION_STABILISHED_TWO_ALTERNATE_RELAYS;
    }

    void GraspingTool::setMsg100()
    {
        current_msg_ = GraspingTool::MSG_TYPE::CONNECTION_STABILISHED_ONE_RELAY;
    }

    void GraspingTool::setMsg101()
    {
        current_msg_ = GraspingTool::MSG_TYPE::ERROR;
        error_cmd_ = true;
    }

    void GraspingTool::setMsg102()
    {
        current_msg_ = GraspingTool::MSG_TYPE::SUCCESS;
        success_cmd_ = true;
    }

    void GraspingTool::setMsg103()
    {
        current_msg_ = GraspingTool::MSG_TYPE::RESET;
        save_cmd_ = false,
        relay_cmd_ = false,
        delete_cmd_ = false;
        error_cmd_ = false;
        success_cmd_ = false;
        reset_cmd_ = true;
    }

    void GraspingTool::callBackGripper()
    {
        relay_cmd_ = !relay_cmd_;
    }

    void GraspingTool::callBackSave()
    {
        save_cmd_ = true;
    }

    void GraspingTool::callbackDelete()
    {
        delete_cmd_ = true;
    }

    bool GraspingTool::load()
    {
        pinMode(RELAY_1, OUTPUT);
        pinMode(RELAY_2, OUTPUT);
        pinMode(BUTTON_CTRL, INPUT);
        pinMode(BUTTON_RECORD, INPUT);
        pinMode(LED_RED, OUTPUT);
        pinMode(LED_BLUE, OUTPUT);
        pinMode(LED_GREEN, OUTPUT);

        Serial.begin(BAUD_RATE);

        // SerialCommand only support functions without parameters (TODO: improve this lib)
        sCmd.addCommand("99", setMsg99);
        sCmd.addCommand("100", setMsg100);
        sCmd.addCommand("101", setMsg101);
        sCmd.addCommand("102", setMsg102);
        sCmd.addCommand("103", setMsg103);

        button_ctrl.attachClick(callBackGripper);
        button_record.attachClick(callBackSave);
        button_record.attachLongPressStop(callbackDelete);

        defineBlinkScheduler();

        return true;
    }

    void GraspingTool::blinkLED(int _led_color)
    {

        this->setLED(LED_COLOR::BLANK);

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

    void GraspingTool::setLED(int _led_color)
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

    void GraspingTool::setRelays(bool _state)
    {

        if (relay_type_ == MSG_TYPE::CONNECTION_STABILISHED_ONE_RELAY)
        {

            digitalWrite(RELAY_1, _state);
        }
        else if (relay_type_ == MSG_TYPE::CONNECTION_STABILISHED_TWO_ALTERNATE_RELAYS)
        {

            digitalWrite(RELAY_1, _state);
            digitalWrite(RELAY_2, !_state);
        }
    }

    void GraspingTool::sendMSG(const char _msg[])
    {
        static long int init_time = millis();
        if (millis() - init_time > 1000)
        {
            Serial.print(millis());
            Serial.print(": ");
            Serial.println(_msg);
            init_time = millis();
        }
    }

    void GraspingTool::sendMSG(MSG_TYPE _msg)
    {

        static long int init_time = millis();
        if (millis() - init_time > 1000)
        {
            //Serial.print(millis());
            //Serial.print(": ");

            Serial.println(_msg);
            init_time = millis();
        }
    }

    void GraspingTool::waitForServerConnection()
    {
        long int init_time;

        this->blinkLED(LED_COLOR::YELLOW);

        while (current_msg_ != MSG_TYPE::CONNECTION_STABILISHED_ONE_RELAY && current_msg_ != MSG_TYPE::CONNECTION_STABILISHED_TWO_ALTERNATE_RELAYS)
        {
            runner_.execute();
            sCmd.readSerial();
            sendMSG("Waiting for server connection.");
        }

        init_time = millis();
        this->blinkLED(LED_COLOR::GREEN);

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

    bool GraspingTool::run()
    {
        current_msg_ = -1;
        this->waitForServerConnection();
        this->initState();
        return true;
    }

    void GraspingTool::initState()
    {
        Serial.println("Current State >> Init State.");

        //Start: State's 0utput
        this->setRelays(LOW); // Suction off or 2F opened
        save_cmd_ = false,
        relay_cmd_ = false,
        delete_cmd_ = false;
        error_cmd_ = false;
        success_cmd_ = false;
        this->setLED(LED_COLOR::WHITE);
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
                run();
            }

            else if (relay_cmd_)
            {
                relayOnState();
            }
            //end: Transitions Verification
        }
    }

    void GraspingTool::relayOnState()
    {
        //Start: State's 0utput
        this->setRelays(HIGH); // Suction ON or 2F CLOSED
        save_cmd_ = false,
        delete_cmd_ = false;
        error_cmd_ = false;
        success_cmd_ = false;
        this->setLED(LED_COLOR::CYAN);
        //End: State's 0utput

        while (true)
        {
            runner_.execute();
            sCmd.readSerial();
            button_ctrl.tick();
            button_record.tick();

            sendMSG("Current State >> Relay ON.");

            if (reset_cmd_)
            {
                Serial.println("Reseting request.");
                run();
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

    void GraspingTool::saveState()
    {
        Serial.println("Current State >> Save State.");

        //Start: State's 0utput
        this->setRelays(HIGH); // Suction ON or 2F CLOSED
        save_cmd_ = false,
        delete_cmd_ = false;
        error_cmd_ = false;
        success_cmd_ = false;
        this->blinkLED(LED_COLOR::BLUE);
        //End: State's 0utput

        while (true)
        {
            runner_.execute();
            sCmd.readSerial();
            button_ctrl.tick();
            button_record.tick();

            sendMSG(MSG_TYPE::SAVE);

            if (reset_cmd_)
            {
                Serial.println("Reseting request.");
                run();
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

    void GraspingTool::errorState()
    {
        Serial.println("Current State >> Error State.");

        //Start: State's 0utput
        this->setRelays(HIGH); // Suction ON or 2F CLOSED
        save_cmd_ = false,
        delete_cmd_ = false;
        error_cmd_ = false;
        success_cmd_ = false;
        this->blinkLED(LED_COLOR::RED);
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
                run();
            }

            else if (!relay_cmd_)
            {
                initState();
            }
        }
    }

    void GraspingTool::successState()
    {
        Serial.println("Current State >> Success State.");

        //Start: State's 0utput
        this->setRelays(HIGH); // Suction ON or 2F CLOSED
        save_cmd_ = false,
        delete_cmd_ = false;
        error_cmd_ = false;
        success_cmd_ = false;
        this->setLED(LED_COLOR::GREEN);
        //End: State's 0utput

        while (true)
        {
            runner_.execute();
            sCmd.readSerial();
            button_ctrl.tick();
            button_record.tick();

            if (reset_cmd_)
            {
                sendMSG("Reseting request.");
                run();
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

    void GraspingTool::cancelState()
    {

        long int init_time;
        Serial.println("Current State >> Cancel State.");

        //Start: State's 0utput
        this->setRelays(HIGH); // Suction ON or 2F CLOSED
        save_cmd_ = false,
        delete_cmd_ = false;
        error_cmd_ = false;
        success_cmd_ = false;
        this->blinkLED(LED_COLOR::MAGENTA);
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
                run();
            }

            else if (success_cmd_)
            {
                sendMSG("Last save canceled.");
                this->blinkLED(LED_COLOR::GREEN);
                init_time = millis();
                while (millis() - init_time < 2000)
                {
                    runner_.execute();
                }
                relayOnState();
            }
        }
    }

}
