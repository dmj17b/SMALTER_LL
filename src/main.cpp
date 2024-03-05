#include <Arduino.h>
#include <Leg.hpp>
#include "TeensyTimerTool.h"
using namespace TeensyTimerTool;

#define kneeMotor_EN 23
#define kneeMotor_DIR 22
#define kneeMotor_ENCA 18
#define kneeMotor_ENCB 19


// Desired motor positions:
float kneeDesPos = 180;

// Create a timer object to control the control loop
PeriodicTimer controlTimer;



// Create a motor object for the knee motor
Motor kneeMotor = Motor(kneeMotor_EN, kneeMotor_DIR, kneeMotor_ENCA, kneeMotor_ENCB);


// Main control function to run every 5ms
void controlFunc(){
  kneeMotor.posControl(kneeDesPos);
  Serial.print("Shaft Position: ");
  Serial.print(kneeMotor.shaftPos());
  Serial.print("  Shaft Velocity: ");
  Serial.println(kneeMotor.shaftVel_);

}

// Setup function
void setup()
{
  Serial.begin(115200);                   // Boot up the serial monitor
  kneeMotor.setGains(3.0, 0.0, 1.0);      // Set PID gains for kneeMotor
  controlTimer.begin(controlFunc, 5000);  // Set the control function to run every 5ms

}

// Main loop
void loop()
{
  // Changing the desired position of the knee motor every 1 second
  // This is just to test the control loop
  kneeDesPos = 180;
  delay(1000);
  kneeDesPos = 0;
  delay(1000);
}

