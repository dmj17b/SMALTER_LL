#include <Arduino.h>
#include <Leg.hpp>
#include "TeensyTimerTool.h"
#include <PulsePosition.h>
using namespace TeensyTimerTool;
#include <Wire.h>

PulsePositionInput ppi;
#define PPM_IN_PIN 9


// Motor pin definitions
// NOT CORRECT YET
#define m1_EN 5
#define m1_DIR 6
#define m1_ENCA 2
#define m1_ENCB 7

#define m2_EN 3
#define m2_DIR 4
#define m2_ENCA 0
#define m2_ENCB 1

#define m3_EN 23
#define m3_DIR 22
#define m3_ENCA 18
#define m3_ENCB 19

#define m4_EN 15
#define m4_DIR 20
#define m4_ENCA 16
#define m4_ENCB 17

// I2C address for the legs:
#define Master Serial6


// Variables for holding desired motor positions:
float m1DesPos = 0;
float m2DesPos = 0;
float m3DesPos = 0;
float m4DesPos = 0;

// Create a timer object to control the control loop
PeriodicTimer controlTimer;

// Create a motor object for the knee motor
Motor m1 = Motor(m1_EN, m1_DIR, m1_ENCA, m1_ENCB);
Motor m2 = Motor(m2_EN, m2_DIR, m2_ENCA, m2_ENCB);
Motor m3 = Motor(m3_EN, m3_DIR, m3_ENCA, m3_ENCB);
Motor m4 = Motor(m4_EN, m4_DIR, m4_ENCA, m4_ENCB);


// Main control function to run every 5ms
void controlFunc(){
  m1.posControl(m1DesPos);
  m2.posControl(m2DesPos);
  m3.posControl(m3DesPos);
  m4.posControl(m4DesPos);
}



// Setup function
void setup()
{
  Serial.begin(115200);            // Boot up the serial monitor
  Master.begin(9600);            // Boot up the serial monitor
  m1.setGains(3.0, 0.0, 1.0);      // Set PID gains for m1
  m2.setGains(3.0, 0.0, 1.0);      // Set PID gains for m2
  m3.setGains(3.0, 0.0, 1.0);      // Set PID gains for m3
  m4.setGains(3.0, 0.0, 1.0);      // Set PID gains for m4
  controlTimer.begin(controlFunc, 5000);  // Set the control function to run every 5ms
  // Wire.onRequest(sendUpdate);

}

int jointIndex;
// Main loop
void loop()
{
  controlTimer.start();
  static float input;
  while(Master.available()>0){
    int d1 = Master.parseInt();
    input = Master.parseFloat();
    jointIndex = d1;
    Serial.println(jointIndex);

    break;
  }
  switch(jointIndex){
    case 0:
      m1.kill();
      m2.kill();
      m3.kill();
      m4.kill();
      controlTimer.stop();
      Serial.println("Killed all motors");
    case 1:
      m1DesPos = input;
      Serial.println(input);
      break;
    case 2:
      m2DesPos = input;
      break;
    case 3:
      m3DesPos = input;
      break;
    case 4:
      m4DesPos = input;
      break;
    default:
      controlTimer.stop();
      break;
  }

}