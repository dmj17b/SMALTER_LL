#include <Arduino.h>
#include <Leg.hpp>
#include "TeensyTimerTool.h"
#include <PulsePosition.h>
using namespace TeensyTimerTool;
#include <Wire.h>

PulsePositionInput ppi;
#define PPM_IN_PIN 9



// Function to update the angle of a joint in a leg through I2C
void updateLeg(int leg_address, int joint, float angle){
  char b[8];
  dtostrf(angle, 4, 2, b);
  Wire2.beginTransmission(leg_address);
  Wire2.write(joint);
  Wire2.write('/');
  Wire2.write(b);
  Wire2.endTransmission();
}

// Setup function
void setup()
{
  Wire2.begin();                  // Join the I2C bus as master
  Serial.begin(115200);            // Boot up the serial monitor
  Wire2.setClock(100000); // Set the I2C clock speed to 100kHz

}

// Main loop
void loop()
{
  // updateLeg(4, 1, 90);
  updateLeg(80, 2, 90);
  // updateLeg(80, 3, 90);


}

