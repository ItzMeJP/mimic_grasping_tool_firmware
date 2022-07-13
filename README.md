
# Grasping Mimic Tool Firmware

* [Overview](#overview)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
* [Configuration](#configuration)
* [Usage](#usage)

# <a name="overview"></a>1. Overview

Grasping mimic tool firmware implementation in C to Arduino. Tested in Arduino Nano Boards. Support to parallel pneumatic two finger and single suction grippers. 

![alt text](/images/mimic_tool.png)
<p align="center">
The Mimic Grasping Handler
</p>

    
# <a name="prerequisites"></a>2. Prerequisites

1. [SerialCommand](https://github.com/ItzMeJP/Arduino-SerialCommand)
2. [TaskScheduler](https://github.com/ItzMeJP/TaskScheduler)
3. [OneButton](https://github.com/ItzMeJP/OneButton)


# <a name="installation"></a>3. Installation

Just compile and upload the code to an Arduino board (the hardreset button into the board should be pressed while uploading). The code was first implemented with VisualCode + PlatformIO extension.

1. [VisualCode](https://code.visualstudio.com/)
2. [PlatformIO](https://platformio.org/install/ide?install=vscode)

# <a name="configuration"></a>3. Configuration

The pinout configuration can be set in "include/header.h" header before compiling the code.

# <a name="usage"></a>3. Usage

The firmware is designed to interact with a external computer with a server working on it. The communication is performed by serial data with a default baudrate of 115200. 

The supported grippers implemented are:
*  FESTO_2F_HGPC_16_A_30; (30 is related to 30mm open width finger)
*  SCHMALZ_FOAM_SUCTION_CUP_FMSW_N10_76x22;

The firmware Finite State Machine (FSM) is presented bellow. 

![alt text](/images/fimrwarefsm.png)
<p align="center">
Firmware FSM.
</p>

1. **Waiting Server:** Initial state waiting for a start command from server. In the message the gripper type need to be defined. << LED Yellow blink color >>
2. **Running:** Tool running rest state which the user can manipulate it. Only the button grasp is enabled allowing the *Grasp* transition. << LED White solid color>>
3. **Active Gripper:** State that active the gripper to grasp.  Both buttons are enabled. Grasp button pressed will activate the *Release* transition. Save button will activate the *Save Request* transition. This state is related to the actuator implemented. Currently, this states only support pneumatic grippers which is perfomed by a relays usages. << LED Cyan color >>
4. **Saving:** State which the firmware will request the server to acquisite data and save it. None of the buttons are enabled. If the server perform the operation succesfully the *Set Success* transition will happen, otherwise, the *Received Error* transition will be activated.  << LED Blue blink color >>
5. **Error:**  Error state. Only Grasp button is enabled allowing the user to release the workobject and leaving the error mode (*Release Transition*) to restart the operation. << LED Red blink color >>
6. **Success:** A state that indicate that the grasping mimic procedure is correctly recorded by the server.  Boths buttons are enables. Grasp button pressed will activate the *Release* transition. Holding save button will activate the *Set Cancel* transition. << LED Green solid color >>
7. **Cancelling**: State which the firmware will request the server to delete the last acquired grasping. The *Canceled* transition will only be activated by a feedback from the server.  << LED Magenta blink color >>

All states can be reseted by a reset message from the server. Thus, the Initial state (**waiting server**) will took place.

The implemented method is a Moore Machine with its outputs described as:

![alt text](/images/output_table.png)
<p align="center">
State's output
</p>



