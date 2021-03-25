#include <Arduino.h>
#include <GraspingTool.h>

using namespace grasping_firmware_tool;

GraspingTool firmware;

void setup() {
  firmware.load(); 
}

void loop() {
  firmware.run();
}


