#include <Arduino.h>
#include <Leg.hpp>
#include "TeensyTimerTool.h"
#include <Motor.hpp>

using namespace TeensyTimerTool;

Leg boomLeg;
Motor kneeMotor = Motor();
void setup() {
  // Setup code goes here
  Serial.begin(115200);
  kneeMotor.init(3, 4, 0, 1);

}

void loop() {
  // Main loop code goes here
    kneeMotor.fwdDrive(100);
    delay(500);
    kneeMotor.fwdDrive(0);
    delay(500);

}
