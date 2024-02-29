#include <Arduino.h>
#include <Leg.hpp>
#include "TeensyTimerTool.h"

using namespace TeensyTimerTool;

Leg boomLeg;

void setup() {
  // Setup code goes here
  Serial.begin(115200);

}

void loop() {
  // Main loop code goes here
    Serial.println(boomLeg.L1_);
    delay(500);
}
