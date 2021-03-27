# Grasping Mimic Tool Firmware

* [Overview](#overview)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
* [Configuration](#configuration)
* [Usage](#usage)

# <a name="overview"></a>1. Overview

Grasping mimic tool firmware implementation in C to Arduino. Tested in Arduino Nano Boards. Support to parallel pneumatic two finger and single suction grippers. 

![alt text](/images/graspingmimictool.jpg)
<p align="center">
A grasping mimic tool
</p>
    
# <a name="prerequisites"></a>2. Prerequisites

1. [SerialCommand](http://wiki.ros.org/eigen3)
2. [TaskScheduler](http://wiki.ros.org/trac_ik)
3. [OneButton](https://github.com/ItzMeJP/universal_robot)


# <a name="installation"></a>3. Installation

Just compile and upload the code to an Arduino board. The code was first implemented with VisualCode + PlatformIO extension.

1. [VisualCode](https://github.com/ItzMeJP/robotiq_arg85_description)
2. [PlatformIO](https://github.com/carlosmccosta/point_cloud_io)

# <a name="configuration"></a>3. Configuration

The pinout configuration can be set in "include/header.h" header before compiling the code.

# <a name="usage"></a>3. Usage

The firmware work following the FSM bellow:
![alt text](/images/firmwarefsm.jpg)
<p align="center">
Firmware FSM.
</p>