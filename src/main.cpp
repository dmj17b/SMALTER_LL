#include <Arduino.h>
#include <Leg.hpp>

Leg boomLeg;

void setup() {
  // Setup code goes here
  Serial.begin(115200);

}

void loop() {
  // Main loop code goes here
    Serial.println(boomLeg.L1_);
}
